#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "esp_err.h"

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
