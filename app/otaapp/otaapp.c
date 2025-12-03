#include "otaapp.h"
#include "esp_log.h"
#include "cJSON.h"
#include "tcp_server.h"

static const char *TAG = "OTA_APP_MGMT";
static ota_task_t pending_task;

void otaapp_clear_pending_task(void) {
    memset(&pending_task, 0, sizeof(pending_task));
    ESP_LOGI(TAG, "Pending task cleared");
}

void ota_dispatch_user_response(const char *client_id, ota_task_t *task, bool accepted) {
    client_info_t *client = client_register_find_by_client_id(client_id);
    if (!client || client->state == CLIENT_OFFLINE) {
        ESP_LOGW(TAG, "Client %s not found or offline, cannot send OTA task", client_id);
        otaapp_clear_pending_task();
        return;
    }

    if (accepted) {
        ESP_LOGI(TAG, "User accepted OTA task %s for client %s", task->task_id, client_id);

        // 构造下发给 ECU 的 JSON
        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "msg_type", "ota_task");
        cJSON_AddStringToObject(root, "task_id", task->task_id);
        cJSON_AddStringToObject(root, "device_name", task->device_name);
        cJSON_AddStringToObject(root, "client_id", task->client_id);
        cJSON_AddStringToObject(root, "version", task->version);
        cJSON_AddStringToObject(root, "firmware_url", task->url);  // 保持和 Client 预期一致
        cJSON_AddStringToObject(root, "features", task->features);

        char *json_str = cJSON_PrintUnformatted(root);
        ESP_LOGI(TAG, "Sending OTA task to client %s (IP=%s): %s", client_id, client->ip, json_str);

        esp_err_t ret = tcp_server_send(client->sock, json_str);

        cJSON_Delete(root);
        free(json_str);

        if (ret == ESP_OK) {
            client->state = CLIENT_UPDATING;
            ESP_LOGI(TAG, "Client %s state updated to UPDATING", client_id);
        } else {
            ESP_LOGE(TAG, "Failed to send OTA task to client %s", client_id);
        }
    } else {
        ESP_LOGW(TAG, "User rejected OTA task %s for client %s", task->task_id, client_id);

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

    otaapp_clear_pending_task();
}

esp_err_t ota_dispatch_send_task(const char *client_id, ota_task_t *task) {
    client_info_t *client = client_register_find_by_client_id(client_id);
    if (!client || client->state == CLIENT_OFFLINE) {
        ESP_LOGW(TAG, "Client %s not found or offline", client_id);
        return ESP_FAIL;
    }

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "msg_type", "ota_task");
    cJSON_AddStringToObject(root, "task_id", task->task_id);
    cJSON_AddStringToObject(root, "device_name", task->device_name);
    cJSON_AddStringToObject(root, "client_id", task->client_id);
    cJSON_AddStringToObject(root, "version", task->version);
    cJSON_AddStringToObject(root, "firmware_url", task->url);
    cJSON_AddStringToObject(root, "features", task->features);

    char *json_str = cJSON_PrintUnformatted(root);
    ESP_LOGI(TAG, "Sending OTA task to client %s (IP=%s): %s", client_id, client->ip, json_str);

    esp_err_t ret = tcp_server_send(client->sock, json_str);

    cJSON_Delete(root);
    free(json_str);

    if (ret == ESP_OK) {
        client->state = CLIENT_UPDATING;
        ESP_LOGI(TAG, "Client %s state updated to UPDATING", client_id);
    }
    return ret;
}
