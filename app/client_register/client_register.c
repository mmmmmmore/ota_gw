#include "client_register.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>

static const char *TAG = "CLIENT_REGISTER";
client_info_t client_list[MAX_CLIENTS];

void client_register_init(void) {
    memset(client_list, 0, sizeof(client_list));
}

static void copy_str(char *dst, size_t dst_sz, const char *src) {
    if (!dst || !dst_sz) return;
    if (!src) { dst[0] = '\0'; return; }
    strncpy(dst, src, dst_sz - 1);
    dst[dst_sz - 1] = '\0';
}

void client_register_save(int sock, cJSON *root) {
    cJSON *dev = cJSON_GetObjectItem(root, "device_name");
    cJSON *cid = cJSON_GetObjectItem(root, "client_id");
    cJSON *mac = cJSON_GetObjectItem(root, "mac");
    cJSON *ver = cJSON_GetObjectItem(root, "version");
    cJSON *ip  = cJSON_GetObjectItem(root, "ip");

    if (!cJSON_IsString(cid)) {
        ESP_LOGW(TAG, "Register missing client_id; ignore");
        return;
    }

    // 优先根据 client_id 查找已有记录
    client_info_t *slot = client_register_find_by_client_id(cid->valuestring);
    if (!slot) {
        // 如果没有找到，尝试根据 MAC 查找
        if (cJSON_IsString(mac)) {
            slot = client_register_find_by_mac(mac->valuestring);
        }
    }
    if (!slot) {
        // 找一个空位
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_list[i].client_id[0] == '\0') {
                slot = &client_list[i];
                break;
            }
        }
    }
    if (!slot) {
        ESP_LOGW(TAG, "Client list full; cannot register %s", cid->valuestring);
        return;
    }

    // 保存字段
    copy_str(slot->device_name, sizeof(slot->device_name), cJSON_IsString(dev) ? dev->valuestring : "");
    copy_str(slot->client_id,   sizeof(slot->client_id),   cid->valuestring);
    copy_str(slot->mac,         sizeof(slot->mac),         cJSON_IsString(mac) ? mac->valuestring : "");
    copy_str(slot->version,     sizeof(slot->version),     cJSON_IsString(ver) ? ver->valuestring : "");
    copy_str(slot->ip,          sizeof(slot->ip),          cJSON_IsString(ip) ? ip->valuestring : "");
    slot->sock  = sock;
    slot->state = CLIENT_ONLINE;

    ESP_LOGI(TAG, "Saving client register: name=%s, id=%s, mac=%s, ip=%s, ver=%s, sock=%d",
             slot->device_name, slot->client_id, slot->mac, slot->ip, slot->version, slot->sock);
}

client_info_t* client_register_find_by_client_id(const char *client_id) {
    if (!client_id) return NULL;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_list[i].client_id[0] && strcmp(client_list[i].client_id, client_id) == 0) {
            return &client_list[i];
        }
    }
    return NULL;
}

client_info_t* client_register_find_by_mac(const char *mac) {
    if (!mac) return NULL;
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_list[i].mac[0] && strcmp(client_list[i].mac, mac) == 0) {
            return &client_list[i];
        }
    }
    return NULL;
}
