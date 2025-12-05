#include "tcp_server.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include <string.h>
#include <unistd.h>
#include "msg_handler.h"
#include "cJSON.h"

static const char *TAG = "GW_TCP_SERVER";
static const char *TAG_OTA = "--OTA_SERVER--";
static const char *TAG_CLT = "--CLient--";

typedef struct {
    int sock;
    msg_role_t role;
    uint32_t last_seen_ms;
} sock_info_t;

#define MAX_SOCKS 10
static sock_info_t sock_table[MAX_SOCKS];
static int ota_server_sock = -1;
static SemaphoreHandle_t s_sock_mutex;

static void lock(void)   { if (s_sock_mutex) xSemaphoreTake(s_sock_mutex, portMAX_DELAY); }
static void unlock(void) { if (s_sock_mutex) xSemaphoreGive(s_sock_mutex); }

void tcp_server_set_ota_sock(int sock) {
    ota_server_sock = sock;
    ESP_LOGI(TAG, "OTA Server socket set: %d", sock);
}

int tcp_server_get_ota_sock(void) {
    return ota_server_sock;
}

static void update_last_seen(int sock) {
    lock();
    for (int i = 0; i < MAX_SOCKS; i++) {
        if (sock_table[i].sock == sock) {
            sock_table[i].last_seen_ms = esp_log_timestamp();
            break;
        }
    }
    unlock();
}

static void register_sock(int sock, msg_role_t role) {
    lock();
    if (role == ROLE_OTA_SERVER && ota_server_sock > 0 && ota_server_sock != sock) {
        ESP_LOGW(TAG, "OTA server already connected on sock=%d, closing old one", ota_server_sock);
        shutdown(ota_server_sock, SHUT_RDWR);
        close(ota_server_sock);
        for (int i = 0; i < MAX_SOCKS; i++) {
            if (sock_table[i].sock == ota_server_sock) {
                sock_table[i].sock = -1;
                sock_table[i].role = ROLE_UNKNOWN;
                sock_table[i].last_seen_ms = 0;
                break;
            }
        }
        ota_server_sock = -1;
    }
    for (int i = 0; i < MAX_SOCKS; i++) {
        if (sock_table[i].sock <= 0) {
            sock_table[i].sock = sock;
            sock_table[i].role = role;
            sock_table[i].last_seen_ms = esp_log_timestamp();
            break;
        }
    }
    if (role == ROLE_OTA_SERVER) {
        ota_server_sock = sock;
        ESP_LOGI(TAG, "OTA Server socket set: %d", sock);
    }
    unlock();
}

static void unregister_sock(int sock) {
    lock();
    for (int i = 0; i < MAX_SOCKS; i++) {
        if (sock_table[i].sock == sock) {
            sock_table[i].sock = -1;
            sock_table[i].role = ROLE_UNKNOWN;
            sock_table[i].last_seen_ms = 0;
            break;
        }
    }
    if (ota_server_sock == sock) {
        ota_server_sock = -1;
        ESP_LOGW(TAG, "OTA Server disconnected");
    }
    unlock();
}

static void set_socket_opts(int sock, int recv_timeout_ms) {
    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
    int idle = 10, intvl = 5, cnt = 3;
    setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE,  &idle, sizeof(idle));
    setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &intvl, sizeof(intvl));
    setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT,   &cnt, sizeof(cnt));
    setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
    struct timeval tv = { .tv_sec = recv_timeout_ms / 1000, .tv_usec = (recv_timeout_ms % 1000) * 1000 };
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    struct linger ling = { .l_onoff = 1, .l_linger = 3 };
    setsockopt(sock, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling));
}

static esp_err_t safe_send(int sock, const char *buf, size_t len) {
    if (sock < 0) return ESP_FAIL;
    int ret = send(sock, buf, len, 0);
    if (ret < 0) {
        ESP_LOGE(TAG, "Send failed on sock=%d: errno=%d", sock, errno);
        shutdown(sock, SHUT_RDWR);
        close(sock);
        unregister_sock(sock);
        return ESP_FAIL;
    }
    return ESP_OK;
}

static void gw_keep_alive_task(void *pvParameters) {
    const char *keep_alive_msg = "{\"msg_type\":\"keep_alive\"}";
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(5000));
        lock();
        int socks[MAX_SOCKS];
        for (int i = 0; i < MAX_SOCKS; i++) socks[i] = sock_table[i].sock;
        unlock();
        for (int i = 0; i < MAX_SOCKS; i++) {
            int sock = socks[i];
            if (sock > 0) safe_send(sock, keep_alive_msg, strlen(keep_alive_msg));
        }
    }
}

static void tcp_server_task(void *pvParameters) {
    int port = (int)(intptr_t)pvParameters;
    ESP_LOGI(TAG, "tcp_server_task started on port %d", port);

    struct sockaddr_in dest_addr = {0};
    dest_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);

    int listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (listen_sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }

    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        close(listen_sock);
        vTaskDelete(NULL);
        return;
    }

    if (listen(listen_sock, 8) < 0) {
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

        set_socket_opts(client_sock, 15000);

        msg_role_t role = (port == 9001) ? ROLE_OTA_SERVER :
                          (port == 9002) ? ROLE_CLIENT : ROLE_UNKNOWN;
        register_sock(client_sock, role);

        char rx_buffer[512];

        while (1) {
            int len = recv(client_sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            if (len < 0) {
                if (errno == EWOULDBLOCK || errno == EAGAIN) {
                    uint32_t now = esp_log_timestamp();
                    uint32_t last = 0;
                    lock();
                    for (int i = 0; i < MAX_SOCKS; i++) {
                        if (sock_table[i].sock == client_sock) {
                            last = sock_table[i].last_seen_ms;
                            break;
                        }
                    }
                    unlock();
                    if (last == 0 || (now - last) > 20000) {
                        ESP_LOGW(TAG, "Sock %d timed out (no keep_alive_ack), closing", client_sock);
                        break;
                        }
                        continue;
                    } else {
                        ESP_LOGE(TAG, "recv failed: errno %d", errno);
                        break;
                    }
                } else if (len == 0) {
                    ESP_LOGW(TAG, "Client disconnected");
                    break;
                } else {
                    rx_buffer[len] = 0;
                    ESP_LOGI(TAG, "Received %d bytes from client %d: %s", len, client_sock, rx_buffer);

                    cJSON *root = cJSON_Parse(rx_buffer);
                    if (root) {
		    			cJSON *msg_type = cJSON_GetObjectItem(root, "msg_type");
			    		if (msg_type && msg_type->valuestring) {
				    		if (strcmp(msg_type->valuestring, "keep_alive_ack") == 0) {
					    		ESP_LOGI(TAG, "[%s] Received keep_alive_ack from sock %d",(role==ROLE_CLIENT ? "CLIENT" : "OTA"), client_sock);

						    	update_last_seen(client_sock);
					    	} else {
                            // 交给消息处理模块
                            ESP_LOGI(TAG, "[%s]Received msg_type =%s from sock %d",(role==ROLE_CLIENT ? "CLIENT" : "OTA"), msg_type->valuestring,client_sock);
                            msg_handler_process(client_sock, rx_buffer, role);
                            update_last_seen(client_sock);
                        }
                    } else {
                        // 没有 msg_type 字段，仍然更新 last_seen
                        update_last_seen(client_sock);
                    }
                    cJSON_Delete(root);
                } else {
                    // 非 JSON 数据，更新 last_seen 以免误判超时
                    update_last_seen(client_sock);
                }
            }
        }
        ESP_LOGI(TAG, "Closing client socket %d", client_sock);
        shutdown(client_sock, SHUT_RDWR);
        close(client_sock);
        unregister_sock(client_sock);
    }
}


esp_err_t tcp_server_send(int client_sock, const char *json_str) {
    if (!json_str || client_sock < 0) return ESP_FAIL;
    return safe_send(client_sock, json_str, strlen(json_str));
}

void gw_tcp_servers_init(void) {
    s_sock_mutex = xSemaphoreCreateMutex();
    memset(sock_table, 0, sizeof(sock_table));
    for (int i = 0; i < MAX_SOCKS; i++) sock_table[i].sock = -1;

    // 分别启动两个端口的服务
    xTaskCreatePinnedToCore(tcp_server_task, "tcp_server_9001", 4096, (void*)(intptr_t)9001, 5, NULL, 0);
    xTaskCreatePinnedToCore(tcp_server_task, "tcp_server_9002", 4096, (void*)(intptr_t)9002, 5, NULL, 0);

    // 心跳任务
    xTaskCreatePinnedToCore(gw_keep_alive_task, "gw_keep_alive_task", 4096, NULL, 4, NULL, 0);
}
