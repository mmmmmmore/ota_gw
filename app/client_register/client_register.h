#ifndef CLIENT_REGISTER_H
#define CLIENT_REGISTER_H

#include "esp_err.h"
#include <stdbool.h>

#define MAX_CLIENTS 6 // 最大客户端数量

// Client 状态枚举
typedef enum {
    CLIENT_OFFLINE = 0,
    CLIENT_ONLINE,
    CLIENT_UPDATING,
    CLIENT_SUCCESS,
    CLIENT_FAIL
} client_state_t;   // ← 改名，不要叫 client_status_t

// Client 信息结构体
typedef struct {
    char device_name[64];  //devices name 
    char client_id[32];
    char mac[18];       // MAC 地址 (格式: "AA:BB:CC:DD:EE:FF")
    char version[32];   // 当前固件版本
    char ip[16];        // IP 地址 (格式: "192.168.4.x")
    client_state_t state; // 当前状态
    int sock;           // TCP 连接的 socket 描述符
} client_info_t;

extern client_info_t client_list[MAX_CLIENTS];

// 初始化 Client 注册表
void client_register_init(void);
// 根据 MAC 查找 Client
client_info_t* client_register_find(const char *mac);

esp_err_t client_register_add(client_info_t *info);
void client_register_remove(const char *mac);

// 添加或更新 Client 信息
esp_err_t client_register_update(const char *mac, const char *ip,
                                 const char *version, client_state_t state,
                                 int sock);


// 打印所有 Client 信息
void client_register_dump(void);



#endif // CLIENT_REGISTER_H

