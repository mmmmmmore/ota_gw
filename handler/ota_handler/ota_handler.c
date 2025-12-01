#include "ota_handler.h"
#include "tcp_server.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>

static const char *TAG = "OTA_HANDLER";

// 全局保存 Client 状态表
static client_status_info_t client_status_list[MAX_CLIENTS];
static int client_count = 0;

// 下发任务给指定 Client ECU
esp_err_t ota_handler_send_task(const char *mac, ota_task_t *task) {
    client_info_t *client = client_register_find(mac);
    if (!client || client->state == CLIENT_OFFLINE) {
        ESP_LOGW(TAG, "Client %s not found or offline", mac);
        return ESP_FAIL;
    }

    // 构造 JSON
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "task", "ota_update");

    // 使用 strlen 检查是否为空字符串，避免传入未初始化内容
    cJSON_AddStringToObject(root, "version",
        (strlen(task->version) > 0) ? task->version : "");
    cJSON_AddStringToObject(root, "url",
        (strlen(task->url) > 0) ? task->url : "");
    cJSON_AddStringToObject(root, "features",
        (strlen(task->features) > 0) ? task->features : "");

    char *json_str = cJSON_PrintUnformatted(root);
    esp_err_t ret = tcp_server_send(client->sock, json_str);

    cJSON_Delete(root);
    free(json_str);

    if (ret == ESP_OK) {
        client->state = CLIENT_UPDATING;
        ESP_LOGI(TAG, "OTA task sent to %s (IP=%s)", mac, client->ip);
    }
    return ret;
}

// 处理来自 OTA Server 的任务 JSON
static void ota_handler_process_task(int sock, cJSON *root) {
    cJSON *version_item = cJSON_GetObjectItem(root, "version");
    cJSON *url_item = cJSON_GetObjectItem(root, "firmware_url");
    cJSON *task_id_item = cJSON_GetObjectItem(root, "task_id");

    if (!cJSON_IsString(version_item) || !cJSON_IsString(url_item)) {
        ESP_LOGE(TAG, "Task JSON missing version/url");
        return;
    }

    const char *version = version_item->valuestring;
    const char *url = url_item->valuestring;
    const char *task_id = (cJSON_IsString(task_id_item)) ? task_id_item->valuestring : "unknown";

    ESP_LOGI(TAG, "Received OTA task from Server: task_id=%s version=%s url=%s",
             task_id, version, url);
    //ack back to OTA server
    // 在 ota_handler_process_task 末尾加：
    const char *ack = "{\"ack\":\"ok\",\"task_id\":\"%s\"}";
    char ack_buf[128];
    snprintf(ack_buf, sizeof(ack_buf), ack, task_id);
    tcp_server_send(sock, ack_buf);
    ESP_LOGI(TAG, "Sent ACK to OTA Server: %s", ack_buf);

    // TODO: 保存任务并分发给 Client ECU
}

// 处理来自 Client ECU 的进度 JSON
static void ota_handler_process_progress(int sock, cJSON *root) {
    cJSON *mac_item = cJSON_GetObjectItem(root, "mac");
    cJSON *progress_item = cJSON_GetObjectItem(root, "progress");
    cJSON *result_item = cJSON_GetObjectItem(root, "result");

    if (!cJSON_IsString(mac_item) || !cJSON_IsNumber(progress_item) || !cJSON_IsString(result_item)) {
        ESP_LOGE(TAG, "Progress JSON missing fields");
        return;
    }

    const char *mac = mac_item->valuestring;
    int progress = progress_item->valueint;
    const char *result = result_item->valuestring;

    client_info_t *client = client_register_find(mac);
    if (client) {
        client_status_info_t *status = &client_status_list[client_count++];
        strncpy(status->client_name, mac, sizeof(status->client_name)-1);
        status->progress = progress;
        status->upgrading = (progress < 100);
        status->last_result = (strcmp(result, "success") == 0);

        ESP_LOGI(TAG, "Client %s progress=%d result=%s", mac, progress, result);

        otaapp_report_result(mac, status->last_result);
    }
}

// 总入口：根据 JSON 内容区分处理
void ota_handler_process_message(int client_sock, const char *json_str) {
    ESP_LOGI(TAG, "Processing message from sock %d: %s", client_sock, json_str);

    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        ESP_LOGE(TAG, "Invalid JSON: %s", json_str);
        return;
    }

    if (cJSON_GetObjectItem(root, "task_id")) {
        ota_handler_process_task(client_sock, root);
    } else if (cJSON_GetObjectItem(root, "mac")) {
        ota_handler_process_progress(client_sock, root);
    } else {
        ESP_LOGW(TAG, "Unknown JSON format");
    }

    cJSON_Delete(root);
}

// 获取所有 Client 的状态信息
client_status_info_t* ota_handler_get_status(int *count) {
    *count = client_count;
    return client_status_list;
}

