#include "tcp_server.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include <string.h>
#include <unistd.h>
#include "msg_handler.h"   // 统一消息处理入口

static const char *TAG = "GW_TCP_SERVER";
static int ota_server_sock = -1;

// 在 init 初始化函数里调用
void gw_tcp_servers_init(void) {
    tcp_server_start(9001);  // OTA Server
    tcp_server_start(9002);  // Client
    xTaskCreate(gw_keep_alive_task, "gw_keep_alive_task", 4096, NULL, 4, NULL);
}

static void gw_keep_alive_task(void *pvParameters) {
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000));  // 每 5 秒

        for (int i = 0; i < MAX_CLIENTS; i++) {
            client_info_t *client = &client_list[i];
            if (client->sock > 0 && client->state != CLIENT_OFFLINE) {
                const char *keep_alive_msg = "{\"msg_type\":\"keep_alive\"}";
                tcp_server_send(client->sock, keep_alive_msg);
                ESP_LOGI(TAG, "Sent keep_alive to client %s (sock=%d)", client->client_id, client->sock);
            }
        }
    }
}




void tcp_server_set_ota_sock(int sock) {
    ota_server_sock = sock;
    ESP_LOGI(TAG, "OTA Server socket set: %d", sock);
}

int tcp_server_get_ota_sock(void) {
    return ota_server_sock;
}

esp_err_t tcp_server_send(int client_sock, const char *json_str) {
    if (client_sock < 0) return ESP_FAIL;
    int err = send(client_sock, json_str, strlen(json_str), 0);
    if (err < 0) {
        ESP_LOGE(TAG, "Send failed: errno %d", errno);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Sent to client %d: %s", client_sock, json_str);
    return ESP_OK;
}

static void tcp_server_task(void *pvParameters) {
    int port = (int)(intptr_t)pvParameters;
    ESP_LOGI(TAG, "tcp_server_task started on port %d", port);

    struct sockaddr_in dest_addr;
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);

    int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (listen_sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    ESP_LOGI(TAG, "Socket created: %d", listen_sock);

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        close(listen_sock);
        vTaskDelete(NULL);
        return;
    }
    ESP_LOGI(TAG, "Socket bound to port: %d", port);

    err = listen(listen_sock, 5);
    if (err < 0) {
        ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
        close(listen_sock);
        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI(TAG, "TCP Server listening on port %d", port);

    while (1) {
        struct sockaddr_in source_addr;
        socklen_t addr_len = sizeof(source_addr);
        int client_sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (client_sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            continue;
        }

        ESP_LOGI(TAG, "Client connected, sock=%d, ip=%s, port=%d",
                 client_sock, inet_ntoa(source_addr.sin_addr), ntohs(source_addr.sin_port));

        // 区分 OTA Server 和 Client
        if (port == 9001) {
            tcp_server_set_ota_sock(client_sock);
        } else {
            ESP_LOGI(TAG, "ECU Client connected on port %d, sock=%d", port, client_sock);
        }

        char rx_buffer[512];
        while (1) {
            int len = recv(client_sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            if (len < 0) {
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                break;
            } else if (len == 0) {
                ESP_LOGW(TAG, "Client disconnected");
                break;
            } else {
                rx_buffer[len] = 0;
                ESP_LOGI(TAG, "Received %d bytes from client %d: %s", len, client_sock, rx_buffer);

                // 统一交给 msg_handler 处理
                if (port == 9001 ) {
                    msg_handler_process(client_sock, rx_buffer, ROLE_OTA_SERVER);
                } else if (port == 9002 ) {
                    msg_handler_process(client_sock, rx_buffer, ROLE_CLIENT);
                } else {
                    msg_handler_process(client_sock, rx_buffer, ROLE_UNKNOWN);
                }
            }
        }

        ESP_LOGI(TAG, "Closing client socket %d", client_sock);
        close(client_sock);

        if (port == 9001 && client_sock == ota_server_sock) {
            ota_server_sock = -1;
            ESP_LOGW(TAG, "OTA Server disconnected");
        }
    }
}

esp_err_t tcp_server_start(uint16_t port) {
    ESP_LOGI(TAG, "Starting TCP server task on port %d", port);
    if (xTaskCreate(tcp_server_task, "tcp_server_task", 4096, (void*)(intptr_t)port, 5, NULL) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create tcp server task");
        return ESP_FAIL;
    }
    return ESP_OK;
}


