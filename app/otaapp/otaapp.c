#include "otaapp.h"
#include "tcp_server.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "OTA_APP_MGMT";

void ota_dispatch_init(void) {
    ESP_LOGI(TAG, "OTA Dispatcher initialized");
    tcp_server_set_rx_callback(tcp_server_rx_handler);
    
}

//1130 update below
// 全局保存当前待确认任务
static ota_task_t pending_task;
static bool has_pending_task = false;

// 设置待确认任务（由 tcp_server 调用）
void otaapp_set_pending_task(ota_task_t *task) {
    if (task) {
        pending_task = *task;   // 拷贝任务内容
        has_pending_task = true;
    }
}

// 获取待确认任务（由 webserver 调用）
ota_task_t* otaapp_get_pending_task(void) {
    if (has_pending_task) {
        return &pending_task;
    }
    return NULL;
}

// 清除待确认任务（用户响应后调用）
void otaapp_clear_pending_task(void) {
    has_pending_task = false;
}


// tcp_server 的统一回调函数
void tcp_server_rx_handler(int client_sock, const char *data) {
    ESP_LOGI(TAG, "Received data from sock %d: %s", client_sock, data);

    // 尝试解析 JSON
    cJSON *root = cJSON_Parse(data);
    if (!root) {
        ESP_LOGW(TAG, "Invalid JSON, maybe ECU message");
        // 如果是 ECU 状态上报，交给 ota_handler
        ota_handler_process_message(client_sock, data);
        return;
    }

    // 判断是否是 OTA Server 的任务
    cJSON *task_item = cJSON_GetObjectItem(root, "task");
    if (task_item && strcmp(task_item->valuestring, "ota_update") == 0) {
        ESP_LOGI(TAG, "Forwarding OTA task to otaapp");
        ota_dispatch_handle_json(data);   // 保存并调度任务
    } else {
        ESP_LOGI(TAG, "Forwarding ECU message to ota_handler");
        ota_handler_process_message(client_sock, data);
    }

    cJSON_Delete(root);
}

// 1130 update above


void ota_dispatch_user_response(const char *mac, ota_task_t *task, bool accepted) {
    if (accepted) {
        ESP_LOGI(TAG, "User accepted OTA task for %s", mac);
        ota_handler_send_task(mac, task);  // 通知 ECU 执行
    } else {
        ESP_LOGW(TAG, "User rejected OTA task for %s", mac);
        // 构造反馈 JSON
        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "task", "ota_update");
        cJSON_AddStringToObject(root, "status", "rejected");
        cJSON_AddStringToObject(root, "version", task->version);
        char *json_str = cJSON_PrintUnformatted(root);

        // 获取 OTA Server socket 并反馈
        int ota_sock = tcp_server_get_ota_sock();
        if (ota_sock >= 0) {
            tcp_server_send(ota_sock, json_str);
        } else {
            ESP_LOGW(TAG, "No OTA Server socket available, cannot send reject result");
        }
        cJSON_Delete(root);
        free(json_str);
    }
    otaapp_clear_pending_task(); // 清除任务
}



// below for handler json info from tcp_server. 
esp_err_t ota_dispatch_handle_json(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        ESP_LOGE(TAG, "Invalid JSON received: %s", json_str);
        return ESP_FAIL;
    }

    ota_task_t task;
    memset(&task, 0, sizeof(task));

    cJSON *ver = cJSON_GetObjectItem(root, "version");
    if (ver && ver->valuestring) {
        strncpy(task.version, ver->valuestring, sizeof(task.version)-1);
    }

    cJSON *url = cJSON_GetObjectItem(root, "url");
    if (url && url->valuestring) {
        strncpy(task.url, url->valuestring, sizeof(task.url)-1);
    }

    cJSON *features = cJSON_GetObjectItem(root, "features");
    if (features && features->valuestring) {
        strncpy(task.features, features->valuestring, sizeof(task.features)-1);
    }

    // 保存为 pending_task，等待用户确认
    otaapp_set_pending_task(&task);

    ESP_LOGI(TAG, "OTA task parsed and pending user confirmation: version=%s, url=%s",
             task.version, task.url);

    cJSON_Delete(root);
    return ESP_OK;
}



esp_err_t ota_dispatch_send_task(const char *mac, ota_task_t *task) {
    client_info_t *client = client_register_find(mac);
    if (!client || client->status == CLIENT_OFFLINE) {
        ESP_LOGW(TAG, "Client %s not found or offline", mac);
        return ESP_FAIL;
    }

    // 构造 JSON
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "task", "ota_update");
    cJSON_AddStringToObject(root, "version", task->version);
    cJSON_AddStringToObject(root, "url", task->url);
    cJSON_AddStringToObject(root, "features", task->features);

    char *json_str = cJSON_PrintUnformatted(root);
    esp_err_t ret = tcp_server_send(client->sock, json_str);

    cJSON_Delete(root);
    free(json_str);

    if (ret == ESP_OK) {
        client->status = CLIENT_UPDATING;
        ESP_LOGI(TAG, "OTA task sent to %s (IP=%s)", mac, client->ip);
    }
    return ret;
}

esp_err_t ota_dispatch_broadcast(ota_task_t *task) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_info_t *client = client_register_find(client_list[i].mac);
        if (client && client->status == CLIENT_ONLINE) {
            ota_dispatch_send_task(client->mac, task);
        }
    }
    return ESP_OK;
}

//ota handler report result to otaapp 
void otaapp_report_result(const char *mac, bool success) {
    ESP_LOGI(TAG, "Reporting result from client %s: %s", 
             mac, success ? "success" : "fail");

    // 构造反馈 JSON
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "task", "ota_update");
    cJSON_AddStringToObject(root, "mac", mac);
    cJSON_AddStringToObject(root, "result", success ? "success" : "fail");

    char *json_str = cJSON_PrintUnformatted(root);

    // 获取 OTA Server socket 并反馈
    int ota_sock = tcp_server_get_ota_sock();
    if (ota_sock >= 0) {
        tcp_server_send(ota_sock, json_str);
    } else {
        ESP_LOGW(TAG, "No OTA Server socket available, cannot report result");
    }

    cJSON_Delete(root);
    free(json_str);
}



