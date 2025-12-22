#include "client_register.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>
#include "tcp_server.h"

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


static const char* client_status_str(client_state_t state){
    switch (state)
    {
        case CLIENT_OFFLINE: return "Offline";
        case CLIENT_ONLINE: return "Online";
        default: return "Offline";
    }
}


client_info_t* client_register_find_by_mac(const char *mac){
    if (!mac) return NULL;
    for (int i=0;i<MAX_CLIENTS;i++){
        if(client_list[i].mac[0] && strcmp(client_list[i].mac, mac)== 0){
            return &client_list[i];
        }
    }
    return NULL;
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
    // create json and send to OTA server
    cJSON *client_register = cJSON_CreateObject();
    cJSON_AddStringToObject(client_register, "msg_type", "register");
    cJSON_AddStringToObject(client_register, "device_name", slot->device_name);
    cJSON_AddStringToObject(client_register, "client_id", slot->client_id);
    cJSON_AddStringToObject(client_register, "mac", slot->mac);
    cJSON_AddStringToObject(client_register, "ip", slot->ip);
    cJSON_AddStringToObject(client_register, "connect_state", client_status_str(slot->state));
    
    char *json_str = cJSON_PrintUnformatted(client_register);

    size_t len = strlen(json_str);
    char *send_buf = malloc(len + 2); 
    if (send_buf) {
        memcpy(send_buf, json_str, len);
        send_buf[len] = '\n';
        send_buf[len+1] = '\0';
    }
    int ota_sock = tcp_server_get_ota_sock();
    if (ota_sock >= 0) {
        ESP_LOGI(TAG, "Sending register data to OTA Server: %s", send_buf);
        tcp_server_send(ota_sock, send_buf);
    } else {
        ESP_LOGW(TAG, "No OTA Server socket available, cannot send online result");
    }
    free(send_buf);
    free(json_str);
    cJSON_Delete(client_register);
            //send register info to ota server
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

void client_offline_info(int client_sock){
    client_info_t *info = NULL;
    for (int i=0;i<MAX_CLIENTS;i++){
        if(client_list[i].sock==client_sock){
            info = &client_list[i];
            break;
        }
    }
    //construct the info to OTA server
    if(!info){
        ESP_LOGW(TAG, "Client : %s, not found by socket: %d", info->client_id, info->sock);
    }

    info->state = CLIENT_OFFLINE;
    cJSON *root_offline= cJSON_CreateObject();
    cJSON_AddStringToObject(root_offline, "msg_type", "register");
    cJSON_AddStringToObject(root_offline, "client_id", info->client_id);
    cJSON_AddStringToObject(root_offline, "connect_state", client_status_str(info->state));

    char *json_str = cJSON_PrintUnformatted(root_offline);

    size_t len = strlen(json_str);
    char *send_buf = malloc(len + 2); 
    if (send_buf) {
        memcpy(send_buf, json_str, len);
        send_buf[len] = '\n';
        send_buf[len+1] = '\0';
    }
    int ota_sock = tcp_server_get_ota_sock();
    if (ota_sock >= 0) {
        ESP_LOGI(TAG, "Sending register data offline info to OTA Server: %s", send_buf);
        tcp_server_send(ota_sock, send_buf);
    } else {
        ESP_LOGW(TAG, "No OTA Server socket available, cannot send offline result");
    }
    free(send_buf);
    free(json_str);
    cJSON_Delete(root_offline);
}