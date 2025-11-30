#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "esp_err.h"

// 启动 TCP Server，监听指定端口
esp_err_t tcp_server_start(uint16_t port);

// 向指定 Client 发送 JSON 消息
esp_err_t tcp_server_send(int client_sock, const char *json_str);

// 注册回调函数，处理 Client 上报的 JSON
typedef void (*tcp_server_rx_callback_t)(int client_id, const char *json_str);
void tcp_server_set_rx_callback(tcp_server_rx_callback_t cb);

#endif // TCP_SERVER_H
