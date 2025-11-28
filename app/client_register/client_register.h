#ifndef CLIENT_REGISTER_H
#define CLIENT_REGISTER_H

#include "esp_err.h"
#include <stdbool.h>

// Client 状态枚举
typedef enum {
    CLIENT_OFFLINE = 0,
    CLIENT_ONLINE,
    CLIENT_UPDATING,
    CLIENT_SUCCESS,
    CLIENT_FAIL
} client_status_t;

// Client 信息结构体
typedef struct {
    char mac[18];       // MAC 地址 (格式: "AA:BB:CC:DD:EE:FF")
    char ip[16];        // IP 地址 (格式: "192.168.4.x")
    char version[32];   // 当前固件版本
    client_status_t status; // 当前状态
    int sock;           // TCP 连接的 socket 描述符
} client_info_t;

// 初始化 Client 注册表
void client_register_init(void);

// 添加或更新 Client 信息
esp_err_t client_register_update(const char *mac, const char *ip,
                                 const char *version, client_status_t status,
                                 int sock);

// 根据 MAC 查找 Client
client_info_t* client_register_find(const char *mac);

// 打印所有 Client 信息
void client_register_dump(void);

#endif // CLIENT_REGISTER_H
