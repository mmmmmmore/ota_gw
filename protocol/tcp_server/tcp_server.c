#include "tcp_server.h"
#include "esp_log.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include <string.h>
#include <unistd.h>
#include "msg_handler.h"
#include "cJSON.h"

#define RX_BUF_SIZE 1024
#define MAX_SOCKS 10


static char rx_buffer[RX_BUF_SIZE];
static int rx_len = 0;

static const char *TAG = "GW_TCP_SERVER";

static sock_info_t sock_table[MAX_SOCKS];
static int ota_server_sock = -1;
static SemaphoreHandle_t s_sock_mutex;

static void lock(void)   { if (s_sock_mutex) xSemaphoreTake(s_sock_mutex, portMAX_DELAY); }
static void unlock(void) { if (s_sock_mutex) xSemaphoreGive(s_sock_mutex); }

static int find_sock_index(int sock){
    for (int i =0; i < MAX_SOCKS; i++){
        if (sock_table[i].sock == sock) return i;
    }
    return -1;
}


void tcp_server_set_ota_sock(int sock) {
    ota_server_sock = sock;
    ESP_LOGI(TAG, "OTA Server socket set: %d", sock);
}

int tcp_server_get_ota_sock(void) {
    return ota_server_sock;
}

static void update_last_seen(int sock) {
    lock();
    int idx = find_sock_index(sock);
    if (idx > 0) sock_table[idx].last_seen_ms = esp_log_timestamp();
    unlock();
}

static void register_sock(int sock, msg_role_t role) {
    lock();
    if (role == ROLE_OTA_SERVER && ota_server_sock > 0 && ota_server_sock != sock) {
        ESP_LOGW(TAG, "OTA server already connected on sock=%d, closing old one", ota_server_sock);
        shutdown(ota_server_sock, SHUT_RDWR);
        close(ota_server_sock);
        int old = find_sock_index(ota_server_sock);
        if (old >= 0) {
            sock_table[old].sock = -1;
            sock_table[old].role = ROLE_UNKNOWN;
            sock_table[old].last_seen_ms = 0;
            sock_table[old].rx_len = 0;
            memset(sock_table[old].rx_buffer, 0, RX_BUF_SIZE);
        }
        ota_server_sock = -1;
    }

    // allocate the slot
    for (int i = 0; i < MAX_SOCKS; i++) {
        if (sock_table[i].sock <= 0) {
            sock_table[i].sock = sock;
            sock_table[i].role = role;
            sock_table[i].last_seen_ms = esp_log_timestamp();
            sock_table[i].rx_len = 0;
            memset(sock_table[i].rx_buffer, 0, RX_BUF_SIZE);
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
    int idx = find_sock_index(sock);
    if (idx >= 0){
        sock_table[idx].sock = -1;
        sock_table[idx].role = ROLE_UNKNOWN;
        sock_table[idx].last_seen_ms = 0;
        sock_table[idx].rx_len = 0;
        memset(sock_table[idx].rx_buffer, 0, RX_BUF_SIZE);
    }

    if (ota_server_sock == sock) {
        ota_server_sock = -1;
        ESP_LOGW(TAG, "OTA Server disconnected");
    }
    unlock();
}


//-------------socket options --------------------------//
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
    const char *keep_alive_msg = "{\"msg_type\":\"keep_alive\"}\n";
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

//----------parse json, confirm msg_type, call msg_handler.c --> msg_handler_process(json) to parse the detail 
static void process_stream_json(int client_sock, msg_role_t role, const char *chunk, int len){
    if (len < 0 ) return;

    lock();
    int idx = find_sock_index(client_sock);
    if (idx < 0 ) { unlock(); return; }

    //append the stream
    sock_info_t *si = &sock_table[idx];
    ESP_LOGI(TAG, "tcp-00-01-process stream json from sock:: %d, role = %d, len = %d  rx_len(before)= %d", 
                    client_sock, role, len, si->rx_len);
    int cap = RX_BUF_SIZE - si->rx_len - 1 ;
    if (len>cap) len =cap;  //prevent overflow
    memcpy(si->rx_buffer+si->rx_len, chunk, len);

    ESP_LOGI(TAG, "tcp-00-02-after memcopy buffer appended, new rx_len = %d , buffers = %s", si->rx_len, si->rx_buffer );
    si->rx_len += len;
    si->rx_buffer[si->rx_len] = '\0' ;

    ESP_LOGI(TAG, "tcp-00-03-before unlocked, new rx_len = %d , buffers = %s", si->rx_len, si->rx_buffer );
    unlock();

    ESP_LOGI(TAG, "tcp-00-04-Rx %d bytes from client %d ::%s , ", len, client_sock, si->rx_buffer);  
    
    //parse loop
    char *start = si->rx_buffer;
    
    while (1)  
    {
        ESP_LOGW(TAG, "tcp-01-01-Parse loop: start offset = %d,  remain = %d",
                    (int)(start- si->rx_buffer), (int)((si->rx_buffer + si->rx_len)-start));
        char *newline = strchr(start, '\n');  // find new line,
        ESP_LOGW(TAG, "tcp-01-02-No newline found, waiting for more data, current buffer = %s ", start);
        if (!newline) break;

        int msg_len = newline - start ; 
        if (msg_len <=0 ){
            start = newline +1;
            continue;
        }
        
        

        //
        if (msg_len >= RX_BUF_SIZE){
            ESP_LOGW(TAG, "tcp-01-03-Single JSON length (- %d -) bigger than handler, drop ", msg_len);
            start = newline +1;
            continue;
        } 
        
        char json_str[RX_BUF_SIZE];
        memcpy(json_str, start, msg_len);
        json_str[msg_len] = '\0';

        ESP_LOGI(TAG, "tcp-01-04-Rx JSON from sock %d :: %s", client_sock, json_str);  

        cJSON *root = cJSON_Parse(json_str);  //rx buffer msg change to json format set as root; 

        if (root) {
            cJSON *msg_type = cJSON_GetObjectItem(root, "msg_type");
            if (msg_type && msg_type->valuestring){
                if (strcmp(msg_type->valuestring, "keep_alive_ack") == 0){
                    ESP_LOGI(TAG, "tcp-02-01-[%s] Rx keep_alive_ack from sock: %d", (role==ROLE_CLIENT ? "CLIENT" : "OTA"), client_sock);
                    update_last_seen(client_sock);
                } else {
                    ESP_LOGI(TAG, "tcp-02-02-[%s] Rx msg_type = %s from sock%d ", (role==ROLE_CLIENT ? "CLIENT" : "OTA"), msg_type->valuestring, client_sock);
                    ESP_LOGI(TAG, "tcp-02-03-dispatching to msg_handler : sock = %d.  role = %d,  json= %s", client_sock, role, json_str);
                    msg_handler_process(client_sock, json_str, role);  // interface between tcp_server and msg_handler 
                    update_last_seen(client_sock);
                   }
            }else {
                ESP_LOGW(TAG, "tcp-02-04-Failed to parse JSON data from sock %d: raw data is %s", client_sock, json_str);
                update_last_seen(client_sock); 
            } 
            cJSON_Delete(root); //clear json object
        } else {
            // 非 JSON 数据，更新 last_seen 以免误判超时
            ESP_LOGW(TAG, "tcp-02-05-Non JSON data from sock %d : raw is %s", client_sock, rx_buffer);
            update_last_seen(client_sock);
    }
    start = newline + 1 ; // to next msg decode process
    }
    //compact remaining
    lock();
    int remain = (si->rx_buffer + si->rx_len) - start ;
    ESP_LOGI(TAG, "tcp-03-01-compacting buffer : %d", remain);
    if (remain > 0 ) memmove(si->rx_buffer, start, remain);
    si->rx_len = remain ;
    si->rx_buffer[si->rx_len] ='\0';
    unlock();

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

        //char rx_buffer[512];

        while (1) {
            char chunk[512];
            int len = recv(client_sock, chunk, sizeof(chunk) ,0);  // 251208 optimize the tcp rx buffer method. 
        //    int len = recv(client_sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            if (len < 0) {
                if (errno == EWOULDBLOCK || errno == EAGAIN) {
                    uint32_t now = esp_log_timestamp();
                    uint32_t last = 0;
                    lock();
                    int idx = find_sock_index(client_sock);
                    if (idx >=0) last = sock_table[idx].last_seen_ms;
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
                    // per-role framing and parse the stream
                    process_stream_json(client_sock, role, chunk, len);
                    
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
    for (int i = 0; i < MAX_SOCKS; i++) {
        sock_table[i].sock = -1;
        sock_table[i].role = ROLE_UNKNOWN;
        sock_table[i].last_seen_ms = 0;
        sock_table[i].rx_len =0;
        memset(sock_table[i].rx_buffer, 0, RX_BUF_SIZE);
    }
    // 分别启动两个端口的服务
    xTaskCreatePinnedToCore(tcp_server_task, "tcp_server_9001", 4096, (void*)(intptr_t)9001, 5, NULL, 0);
    xTaskCreatePinnedToCore(tcp_server_task, "tcp_server_9002", 4096, (void*)(intptr_t)9002, 5, NULL, 0);

    // 心跳任务
    xTaskCreatePinnedToCore(gw_keep_alive_task, "gw_keep_alive_task", 4096, NULL, 4, NULL, 0);
}
