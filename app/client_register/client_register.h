#ifndef CLIENT_REGISTER_H
#define CLIENT_REGISTER_H

#include "esp_err.h"
#include <stdbool.h>
#include "cJSON.h"   // 新增：因为 save 接口需要解析 JSON

#define MAX_CLIENTS 6 // 最大客户端数量

// Client 状态枚举
typedef enum {
    CLIENT_OFFLINE = 0,
    CLIENT_ONLINE,
    CLIENT_UPDATING,
    CLIENT_SUCCESS,
    CLIENT_FAIL
} client_state_t;

// Client 信息结构体
typedef struct {
    char device_name[64];   // 设备名称
    char client_id[32];     // 客户端 ID
    char mac[18];           // MAC 地址 (格式: "AA:BB:CC:DD:EE:FF")
    char version[32];       // 当前固件版本
    char ip[16];            // IP 地址 (格式: "192.168.4.x")
    client_state_t state;   // 当前状态
    int sock;               // TCP 连接的 socket 描述符
} client_info_t;


extern client_info_t client_list[MAX_CLIENTS];

// 初始化
void client_register_init(void);

// 保存注册信息
void client_register_save(int sock, cJSON *root);

// 查找函数
client_info_t* client_register_find_by_client_id(const char *client_id);
client_info_t* client_register_find_by_mac(const char *mac);

#endif // CLIENT_REGISTER_H

