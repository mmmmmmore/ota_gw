#include "ota_handler.h"
#include "tcp_server.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "OTA_HANDLER";

// 全局保存 Client 状态表
static client_status_info_t client_status_list[MAX_CLIENTS];
static int client_count = 0;

// 下发任务给指定 Client ECU
esp_err_t ota_handler_send_task(const char *mac, ota_task_t *task) {
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
        client->state = CLIENT_UPDATING;
        ESP_LOGI(TAG, "OTA task sent to %s (IP=%s)", mac, client->ip);
    }
    return ret;
}

// 处理来自 Client ECU 的上报消息
void ota_handler_process_message(int client_sock, const char *json_str) {
    ESP_LOGI(TAG, "Processing ECU message from sock %d: %s", client_sock, json_str);

    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        ESP_LOGE(TAG, "Invalid ECU JSON: %s", json_str);
        return;
    }

    const char *mac = cJSON_GetObjectItem(root, "mac")->valuestring;
    int progress = cJSON_GetObjectItem(root, "progress")->valueint;
    const char *result = cJSON_GetObjectItem(root, "result")->valuestring;

    client_info_t *client = client_register_find(mac);
    if (client) {
        // 更新状态表
        client_status_info_t *status = &client_status_list[client_count++];
        strncpy(status->client_name, mac, sizeof(status->client_name)-1);
        status->progress = progress;
        status->upgrading = (progress < 100);
        status->last_result = (strcmp(result, "success") == 0);

        ESP_LOGI(TAG, "Client %s progress=%d result=%s", mac, progress, result);

        // 上报给 otaapp
        otaapp_report_result(mac, status->last_result);
    }

    cJSON_Delete(root);
}

// 获取所有 Client 的状态信息
client_status_info_t* ota_handler_get_status(int *count) {
    *count = client_count;
    return client_status_list;
}

