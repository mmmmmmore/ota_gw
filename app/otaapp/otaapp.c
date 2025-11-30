#include "otaapp.h"
#include "tcp_server.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "OTA_DISPATCH";

void ota_dispatch_init(void) {
    ESP_LOGI(TAG, "OTA Dispatcher initialized");
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

        // 反馈给 OTA Server
        tcp_server_send(/*ota_server_sock*/, json_str);

        cJSON_Delete(root);
        free(json_str);
    }
}



// below for handler json info from tcp_server. 
esp_err_t ota_dispatch_handle_json(const char *json_str) {
    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        ESP_LOGE(TAG, "Invalid JSON received: %s", json_str);
        return ESP_FAIL;
    }

    // 提取任务信息
    ota_task_t task;
    memset(&task, 0, sizeof(task));
    strncpy(task.version, cJSON_GetObjectItem(root, "version")->valuestring, sizeof(task.version)-1);
    strncpy(task.url, cJSON_GetObjectItem(root, "url")->valuestring, sizeof(task.url)-1);
    strncpy(task.features, cJSON_GetObjectItem(root, "features")->valuestring, sizeof(task.features)-1);

    // 提取目标设备
    cJSON *mac_item = cJSON_GetObjectItem(root, "mac");
    if (mac_item && mac_item->valuestring) {
        ota_dispatch_send_task(mac_item->valuestring, &task);
    } else {
        ESP_LOGW(TAG, "No MAC specified, broadcasting task");
        ota_dispatch_broadcast(&task);
    }

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



