#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "esp_err.h"
#include "msg_handler.h"

#define RX_BUF_SIZE 1024
#define MAX_SOCKS 10

typedef struct {
    int sock;
    msg_role_t role;
    uint32_t last_seen_ms;
    char rx_buffer[RX_BUF_SIZE];
    int rx_len;
} sock_info_t;

// 初始化：启动两个端口监听
void gw_tcp_servers_init(void);

// 启动一个 TCP Server 任务（内部使用）
esp_err_t tcp_server_start(uint16_t port);

// OTA Server socket 管理
void tcp_server_set_ota_sock(int sock);
int tcp_server_get_ota_sock(void);

// 发送 JSON 数据
esp_err_t tcp_server_send(int client_sock, const char *json_str);

#endif // TCP_SERVER_H
