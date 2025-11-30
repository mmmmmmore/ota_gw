#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "esp_err.h"

// 回调函数类型定义
typedef void (*tcp_server_rx_callback_t)(int client_sock, const char *data);

// 设置接收回调函数
void tcp_server_set_rx_callback(tcp_server_rx_callback_t cb);

// 启动 TCP Server
esp_err_t tcp_server_start(uint16_t port);

// 发送数据到指定客户端
esp_err_t tcp_server_send(int client_sock, const char *json_str);

// 设置 OTA Server socket
void tcp_server_set_ota_sock(int sock);

// 获取 OTA Server socket
int tcp_server_get_ota_sock(void);

#endif // TCP_SERVER_H


