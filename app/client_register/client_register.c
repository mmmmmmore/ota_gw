#include "client_register.h"
#include "esp_log.h"
#include <string.h>
#include "cJSON.h"

//#define MAX_CLIENTS 10
static const char *TAG = "CLIENT_REGISTER";

static client_info_t client_list[MAX_CLIENTS];

void client_register_init(void) {
    memset(client_list, 0, sizeof(client_list));
    ESP_LOGI(TAG, "Client register initialized");
}

esp_err_t client_register_update(const char *mac, const char *ip,
                                 const char *version, client_status_t status,
                                 int sock) {
    // 查找是否已有该 MAC
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (strcmp(client_list[i].mac, mac) == 0) {
            strncpy(client_list[i].ip, ip, sizeof(client_list[i].ip));
            strncpy(client_list[i].version, version, sizeof(client_list[i].version));
            client_list[i].status = status;
            client_list[i].sock = sock;
            ESP_LOGI(TAG, "Updated client %s (IP=%s, Ver=%s, Status=%d)",
                     mac, ip, version, status);
            return ESP_OK;
        }
    }

    // 如果没有，添加新 Client
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (strlen(client_list[i].mac) == 0) {
            strncpy(client_list[i].mac, mac, sizeof(client_list[i].mac));
            strncpy(client_list[i].ip, ip, sizeof(client_list[i].ip));
            strncpy(client_list[i].version, version, sizeof(client_list[i].version));
            client_list[i].status = status;
            client_list[i].sock = sock;
            ESP_LOGI(TAG, "Added new client %s (IP=%s, Ver=%s, Status=%d)",
                     mac, ip, version, status);
            return ESP_OK;
        }
    }

    ESP_LOGW(TAG, "Client list full, cannot add %s", mac);
    return ESP_FAIL;
}

client_info_t* client_register_find(const char *mac) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (strcmp(client_list[i].mac, mac) == 0) {
            return &client_list[i];
        }
    }
    return NULL;
}

void client_register_dump(void) {
    ESP_LOGI(TAG, "Dumping client list:");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (strlen(client_list[i].mac) > 0) {
            ESP_LOGI(TAG, "Client[%d]: MAC=%s, IP=%s, Ver=%s, Status=%d, Sock=%d",
                     i,
                     client_list[i].mac,
                     client_list[i].ip,
                     client_list[i].version,
                     client_list[i].status,
                     client_list[i].sock);
        }
    }
}
