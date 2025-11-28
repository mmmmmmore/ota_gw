#include "ota_handler.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>

static const char *TAG = "OTA_HANDLER";

// 假设最多支持 4 个 Client
#define MAX_CLIENTS 4
static client_status_t clients[MAX_CLIENTS];

void ota_handler_init(void) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        snprintf(clients[i].client_name, sizeof(clients[i].client_name), "Client_%d", i+1);
        clients[i].partition = PARTITION_A;
        clients[i].upgrading = false;
        clients[i].progress = 0;
        clients[i].last_result = false;
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
            ESP_LOGI(TAG, "Client %s starts upgrading...", client);
            break;
        }
    }

    cJSON_Delete(root);
}

// 模拟进度更新
void ota_handler_update_progress(void) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].upgrading) {
            if (clients[i].progress < 100) {
                clients[i].progress += 10; // 每次增加 10%
                ESP_LOGI(TAG, "Client %s progress: %d%%", clients[i].client_name, clients[i].progress);
            } else {
                // 升级完成，模拟成功
                clients[i].upgrading = false;
                clients[i].last_result = true;
                clients[i].partition = PARTITION_B;
                ESP_LOGI(TAG, "Client %s upgrade SUCCESS, switched to Partition B", clients[i].client_name);
            }
        }
    }
}

// 获取状态表
client_status_t *ota_handler_get_status(int *count) {
    *count = MAX_CLIENTS;
    return clients;
}
