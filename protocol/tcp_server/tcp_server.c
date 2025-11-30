#include "tcp_server.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include <string.h>
#include <unistd.h>
#include "otaapp.h"

static const char *TAG = "GW_TCP_SERVER";
static tcp_server_rx_callback_t rx_callback = NULL;

void tcp_server_set_rx_callback(tcp_server_rx_callback_t cb) {
    rx_callback = cb;
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

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err < 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        close(listen_sock);
        vTaskDelete(NULL);
        return;
    }

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
        uint addr_len = sizeof(source_addr);
        int client_sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (client_sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            continue;
        }
        ESP_LOGI(TAG, "Client connected, sock=%d", client_sock);

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
                if (rx_callback) {
                    rx_callback(client_sock, rx_buffer);
                    ESP_LOGI(TAG, "Forward OTA task json to otaapp")
                    ota_dispatch_handle_json(rx_buffer);
                }
            }
        }
        close(client_sock);
    }
}

esp_err_t tcp_server_start(uint16_t port) {
    xTaskCreate(tcp_server_task, "tcp_server_task", 4096, (void*)(intptr_t)port, 5, NULL);
    return ESP_OK;
}
