#include "otaapp.h"
#include "tcp_server.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>

static const char *TAG = "OTA_APP_MGMT";

// ---------- 全局保存当前待确认任务 ----------
static ota_task_t pending_task;
static bool has_pending_task = false;

void ota_dispatch_init(void) {
    ESP_LOGI(TAG, "OTA Dispatcher initialized");
}

// 设置待确认任务
void otaapp_set_pending_task(ota_task_t *task) {
    if (task) {
        pending_task = *task;   // 拷贝任务内容
        has_pending_task = true;
        ESP_LOGI(TAG, "Task stored as pending: version=%s url=%s features=%s",
                 task->version, task->url, task->features);
    }
}

// 获取待确认任务
ota_task_t* otaapp_get_pending_task(void) {
    if (has_pending_task) {
        ESP_LOGI(TAG, "Returning pending task: version=%s url=%s",
                 pending_task.version, pending_task.url);
        return &pending_task;
    }
    ESP_LOGI(TAG, "No pending task available");
    return NULL;
}

// 清除待确认任务
void otaapp_clear_pending_task(void) {
    has_pending_task = false;
    ESP_LOGI(TAG, "Pending task cleared");
}

// ---------- 处理 OTA Server 下发的任务（由 msg_handler 调用） ----------
void otaapp_process_task(int sock, cJSON *root) {
    cJSON *task_id_item = cJSON_GetObjectItem(root, "task_id");
    cJSON *version_item = cJSON_GetObjectItem(root, "version");
    cJSON *url_item = cJSON_GetObjectItem(root, "firmware_url");

    if (!cJSON_IsString(task_id_item) || !cJSON_IsString(version_item) || !cJSON_IsString(url_item)) {
        ESP_LOGE(TAG, "OTA task JSON missing fields");
        return;
    }

    ota_task_t task;
    memset(&task, 0, sizeof(task));
    strncpy(task.task_id, task_id_item->valuestring, sizeof(task.task_id)-1);
    strncpy(task.version, version_item->valuestring, sizeof(task.version)-1);
    strncpy(task.url, url_item->valuestring, sizeof(task.url)-1);

    cJSON *features_item = cJSON_GetObjectItem(root, "features");
    if (cJSON_IsString(features_item)) {
        strncpy(task.features, features_item->valuestring, sizeof(task.features)-1);
    }
    cJSON *dev_item = cJSON_GetObjectItem(root, "device_name");
    if (cJSON_IsString(dev_item)) {
        strncpy(task.device_name, dev_item->valuestring, sizeof(task.device_name)-1);
    }
    cJSON *cid_item = cJSON_GetObjectItem(root, "client_id");
    if (cJSON_IsString(cid_item)) {
        strncpy(task.client_id, cid_item->valuestring, sizeof(task.client_id)-1);
    }

    otaapp_set_pending_task(&task);

    ESP_LOGI(TAG, "OTA task parsed and pending user confirmation: id=%s version=%s url=%s",
             task.task_id, task.version, task.url);

    // 回复 ACK 给 OTA Server
    char ack_buf[128];
    snprintf(ack_buf, sizeof(ack_buf), "{\"ack\":\"ok\",\"task_id\":\"%s\"}", task.task_id);
    tcp_server_send(sock, ack_buf);
    ESP_LOGI(TAG, "Sent ACK to OTA Server: %s", ack_buf);
}

// ---------- 用户Accept之后对目标ECU做更新 ----------
void ota_dispatch_user_response(const char *mac, ota_task_t *task, bool accepted) {
    if (accepted) {
        ESP_LOGI(TAG, "User accepted OTA task %s for client %s", task->task_id, mac);

        client_info_t *client = client_register_find(mac);
        if (!client || client->state == CLIENT_OFFLINE) {
            ESP_LOGW(TAG, "Client %s not found or offline, cannot send OTA task", mac);
            otaapp_clear_pending_task();
            return;
        }

        // 构造下发给 ECU 的 JSON
        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "task_id", task->task_id);
        cJSON_AddStringToObject(root, "device_name", task->device_name);
        cJSON_AddStringToObject(root, "client_id", task->client_id);
        cJSON_AddStringToObject(root, "version", task->version);
        cJSON_AddStringToObject(root, "url", task->url);

        char *json_str = cJSON_PrintUnformatted(root);
        ESP_LOGI(TAG, "Sending OTA task to ECU %s (IP=%s): %s", mac, client->ip, json_str);

        esp_err_t ret = tcp_server_send(client->sock, json_str);

        cJSON_Delete(root);
        free(json_str);

        if (ret == ESP_OK) {
            client->state = CLIENT_UPDATING;
            ESP_LOGI(TAG, "Client %s state updated to UPDATING", mac);
        } else {
            ESP_LOGE(TAG, "Failed to send OTA task to client %s", mac);
        }
    } else {
        ESP_LOGW(TAG, "User rejected OTA task %s for client %s", task->task_id, mac);

        // 构造反馈 JSON 给 OTA Server
        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "task_id", task->task_id);
        cJSON_AddStringToObject(root, "status", "rejected");
        cJSON_AddStringToObject(root, "version", task->version);
        char *json_str = cJSON_PrintUnformatted(root);

        int ota_sock = tcp_server_get_ota_sock();
        if (ota_sock >= 0) {
            ESP_LOGI(TAG, "Sending reject result to OTA Server: %s", json_str);
            tcp_server_send(ota_sock, json_str);
        } else {
            ESP_LOGW(TAG, "No OTA Server socket available, cannot send reject result");
        }
        cJSON_Delete(root);
        free(json_str);
    }

    otaapp_clear_pending_task(); // 清除任务
}

// ---------- 下发任务给指定 ECU ----------
esp_err_t ota_dispatch_send_task(const char *mac, ota_task_t *task) {
    client_info_t *client = client_register_find(mac);
    if (!client || client->state == CLIENT_OFFLINE) {
        ESP_LOGW(TAG, "Client %s not found or offline", mac);
        return ESP_FAIL;
    }

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "task", "ota_update");
    cJSON_AddStringToObject(root, "version", task->version);
    cJSON_AddStringToObject(root, "url", task->url);
    cJSON_AddStringToObject(root, "features", task->features);

    char *json_str = cJSON_PrintUnformatted(root);
    ESP_LOGI(TAG, "Sending OTA task to client %s (IP=%s): %s", mac, client->ip, json_str);

    esp_err_t ret = tcp_server_send(client->sock, json_str);

    cJSON_Delete(root);
    free(json_str);

    if (ret == ESP_OK) {
        client->state = CLIENT_UPDATING;
        ESP_LOGI(TAG, "Client %s state updated to UPDATING", mac);
    }
    return ret;
}

// ---------- 广播任务给所有在线 ECU ----------
esp_err_t ota_dispatch_broadcast(ota_task_t *task) {
    ESP_LOGI(TAG, "Broadcasting OTA task version=%s to all online clients", task->version);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (strlen(client_list[i].mac) > 0 && client_list[i].state == CLIENT_ONLINE) {
            ota_dispatch_send_task(client_list[i].mac, task);
        }
    }
    return ESP_OK;
}

// ---------- ECU 上报结果，转发给 OTA Server ----------
void otaapp_report_result(const char *mac, bool success) {
    ESP_LOGI(TAG, "Reporting result from client %s: %s",
             mac, success ? "success" : "fail");

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "task", "ota_update");
    cJSON_AddStringToObject(root, "mac", mac);
    cJSON_AddStringToObject(root, "result", success ? "success" : "fail");

    char *json_str = cJSON_PrintUnformatted(root);

    int ota_sock = tcp_server_get_ota_sock();
    if (ota_sock >= 0) {
        ESP_LOGI(TAG, "Sending result to OTA Server: %s", json_str);
        tcp_server_send(ota_sock, json_str);
    } else {
        ESP_LOGW(TAG, "No OTA Server socket available, cannot report result");
    }

    cJSON_Delete(root);
    free(json_str);
}
