#include "ota_dispatch.h"
#include "tcp_server.h"
#include "esp_log.h"
#include "cJSON.h"

static const char *TAG = "OTA_DISPATCH";

void ota_dispatch_init(void) {
    ESP_LOGI(TAG, "OTA Dispatcher initialized");
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
