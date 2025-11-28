#include "ota_handler.h"
#include "esp_log.h"
#include "cJSON.h"
#include "esp_timer.h"
#include <string.h>

static const char *TAG = "OTA_HANDLER";

#define MAX_CLIENTS 4
#define OTA_TIMEOUT_MS 60000  // 1分钟超时

static client_status_t clients[MAX_CLIENTS];

void ota_handler_init(void) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        snprintf(clients[i].client_name, sizeof(clients[i].client_name), "Client_%d", i+1);
        clients[i].partition = PARTITION_A;
        clients[i].upgrading = false;
        clients[i].progress = 0;
        clients[i].last_result = false;
        clients[i].start_time = 0;
    }
    ESP_LOGI(TAG, "OTA Handler initialized, all clients on Partition A");
}

void ota_handler_process(const char *task_json) {
    cJSON *root = cJSON_Parse(task_json);
    if (!root) {
        ESP_LOGE(TAG, "Invalid JSON task");
        return;
    }

    const char *client = cJSON_GetObjectItem(root, "client")->valuestring;
    const char *version = cJSON_GetObjectItem(root, "version")->valuestring;
    const char *url = cJSON_GetObjectItem(root, "url")->valuestring;

    ESP_LOGI(TAG, "OTA Task: client=%s, version=%s, url=%s", client, version, url);

    // 找到对应 Client
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (strcmp(clients[i].client_name, client) == 0) {
            clients[i].upgrading = true;
            clients[i].progress = 0;
            clients[i].last_result = false;
            clients[i].start_time = esp_timer_get_time() / 1000; // 毫秒
            ESP_LOGI(TAG, "Client %s starts upgrading...", client);
            break;
        }
    }

    cJSON_Delete(root);
}

void ota_handler_update_progress(void) {
    uint64_t now = esp_timer_get_time() / 1000; // 毫秒
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].upgrading) {
            if (clients[i].progress < 100) {
                // 检查超时
                if (now - clients[i].start_time > OTA_TIMEOUT_MS) {
                    clients[i].upgrading = false;
                    clients[i].last_result = false;
                    ESP_LOGW(TAG, "Client %s OTA TIMEOUT, upgrade FAILED", clients[i].client_name);
                } else {
                    clients[i].progress += 10; // 模拟进度
                    ESP_LOGI(TAG, "Client %s progress: %d%%", clients[i].client_name, clients[i].progress);
                }
            } else {
                // 升级完成
                clients[i].upgrading = false;
                clients[i].last_result = true;
                clients[i].partition = PARTITION_B;
                ESP_LOGI(TAG, "Client %s upgrade SUCCESS, switched to Partition B", clients[i].client_name);
            }
        }
    }
}

client_status_t *ota_handler_get_status(int *count) {
    *count = MAX_CLIENTS;
    return clients;
}
