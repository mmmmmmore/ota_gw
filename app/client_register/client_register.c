#include "client_register.h"
#include "esp_log.h"
#include <string.h>
#include "cJSON.h"

static const char *TAG = "CLIENT_REGISTER";

// 全局客户端列表
client_info_t client_list[MAX_CLIENTS];

void client_register_init(void) {
    memset(client_list, 0, sizeof(client_list));
    ESP_LOGI(TAG, "Client register initialized");
}

// 添加新客户端
esp_err_t client_register_add(client_info_t *info) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (strlen(client_list[i].mac) == 0) {
            client_list[i] = *info;
            client_list[i].state = CLIENT_ONLINE;
            ESP_LOGI(TAG, "Added new client: name=%s, id=%s, mac=%s, ip=%s, ver=%s, sock=%d",
                     info->device_name, info->client_id, info->mac,
                     info->ip, info->version, info->sock);
            return ESP_OK;
        }
    }
    ESP_LOGW(TAG, "Client list full, cannot add %s", info->mac);
    return ESP_FAIL;
}

// 更新客户端信息（存在则更新，不存在则添加）
esp_err_t client_register_update(const char *mac, const char *ip,
                                 const char *version, client_state_t state,
                                 int sock) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (strcmp(client_list[i].mac, mac) == 0) {
            strncpy(client_list[i].ip, ip, sizeof(client_list[i].ip)-1);
            strncpy(client_list[i].version, version, sizeof(client_list[i].version)-1);
            client_list[i].state = state;
            client_list[i].sock = sock;
            ESP_LOGI(TAG, "Updated client %s (IP=%s, Ver=%s, State=%d, Sock=%d)",
                     mac, ip, version, state, sock);
            return ESP_OK;
        }
    }

    // 没找到则添加
    client_info_t info = {0};
    strncpy(info.mac, mac, sizeof(info.mac)-1);
    strncpy(info.ip, ip, sizeof(info.ip)-1);
    strncpy(info.version, version, sizeof(info.version)-1);
    info.state = state;
    info.sock = sock;
    return client_register_add(&info);
}

// 根据 MAC 查找客户端
client_info_t* client_register_find(const char *mac) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (strcmp(client_list[i].mac, mac) == 0) {
            return &client_list[i];
        }
    }
    return NULL;
}

// 移除客户端
void client_register_remove(const char *mac) {
    client_info_t *client = client_register_find(mac);
    if (client) {
        ESP_LOGI(TAG, "Removing client: mac=%s, name=%s, id=%s", 
                 client->mac, client->device_name, client->client_id);
        memset(client, 0, sizeof(client_info_t));
    }
}

// 打印所有客户端信息
void client_register_dump(void) {
    ESP_LOGI(TAG, "Dumping client list:");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (strlen(client_list[i].mac) > 0) {
            ESP_LOGI(TAG, "Client[%d]: name=%s, id=%s, mac=%s, ip=%s, ver=%s, state=%d, sock=%d",
                     i,
                     client_list[i].device_name,
                     client_list[i].client_id,
                     client_list[i].mac,
                     client_list[i].ip,
                     client_list[i].version,
                     client_list[i].state,
                     client_list[i].sock);
        }
    }
}

// 新增：由 msg_handler 调用，解析注册 JSON 并保存
esp_err_t client_register_save(int sock, cJSON *root) {
    cJSON *mac_item = cJSON_GetObjectItem(root, "mac");
    cJSON *device_item = cJSON_GetObjectItem(root, "device_name");
    cJSON *client_id_item = cJSON_GetObjectItem(root, "client_id");
    cJSON *version_item = cJSON_GetObjectItem(root, "version");
    cJSON *ip_item = cJSON_GetObjectItem(root, "ip");

    if (!cJSON_IsString(mac_item) || !cJSON_IsString(device_item)) {
        ESP_LOGE(TAG, "Register JSON missing fields");
        return ESP_FAIL;
    }

    client_info_t info = {0};
    strncpy(info.mac, mac_item->valuestring, sizeof(info.mac)-1);
    strncpy(info.device_name, device_item->valuestring, sizeof(info.device_name)-1);
    if (cJSON_IsString(client_id_item))
        strncpy(info.client_id, client_id_item->valuestring, sizeof(info.client_id)-1);
    if (cJSON_IsString(version_item))
        strncpy(info.version, version_item->valuestring, sizeof(info.version)-1);
    if (cJSON_IsString(ip_item))
        strncpy(info.ip, ip_item->valuestring, sizeof(info.ip)-1);

    info.sock = sock;
    info.state = CLIENT_ONLINE;

    ESP_LOGI(TAG, "Saving client register: name=%s, id=%s, mac=%s, ip=%s, ver=%s, sock=%d",
             info.device_name, info.client_id, info.mac, info.ip, info.version, info.sock);

    return client_register_update(info.mac, info.ip, info.version, info.state, info.sock);
}
