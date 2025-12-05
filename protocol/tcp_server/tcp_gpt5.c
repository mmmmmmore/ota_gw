/* file: gw_tcp_server.c
   Compile with ESP-IDF (or adapt for other lwIP environments)
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>    // or FreeRTOS tasks; adapt as needed
#include <stdint.h>

#define SERVER_PORT 9001
#define BACKLOG 5
#define MAX_PAYLOAD (1024*8)

static int server_sock = -1;
static int client_sock = -1;
static pthread_mutex_t client_lock = PTHREAD_MUTEX_INITIALIZER;

/* Helper: set socket non-blocking if needed
   (we use blocking recv in recv_task so leave blocking)
*/

static void safe_close_client() {
    pthread_mutex_lock(&client_lock);
    if (client_sock >= 0) {
        shutdown(client_sock, SHUT_RDWR);
        close(client_sock);
        client_sock = -1;
    }
    pthread_mutex_unlock(&client_lock);
}

/* Replace client sock (close old, set new) */
static void set_new_client(int s) {
    pthread_mutex_lock(&client_lock);
    if (client_sock >= 0) {
        printf("[GW] Close old client socket %d\n", client_sock);
        shutdown(client_sock, SHUT_RDWR);
        close(client_sock);
    }
    client_sock = s;
    printf("[GW] New client socket set: %d\n", client_sock);
    pthread_mutex_unlock(&client_lock);
}

/* read exactly n bytes (blocking) */
static int recv_exact(int sock, void *buf, int n) {
    int total = 0;
    char *p = (char*)buf;
    while (total < n) {
        int r = recv(sock, p + total, n - total, 0);
        if (r <= 0) return r; // 0 closed, -1 error
        total += r;
    }
    return total;
}

static void handle_json_message(int sock, const char *json, int len) {
    // naive parse for msg_type field (for demo). In real use, use cJSON or similar.
    // For simplicity, we'll just search for "msg_type":"xxx"
    printf("[GW] Received %d bytes from sock %d: %.*s\n", len, sock, len, json);

    if (strstr(json, "\"msg_type\":\"keep_alive\"") != NULL) {
        // respond keep_alive_ack
        const char *ack = "{\"msg_type\":\"keep_alive_ack\"}";
        uint32_t l = htonl(strlen(ack));
        send(sock, &l, 4, 0);
        send(sock, ack, strlen(ack), 0);
        printf("[GW] Sent keep_alive_ack to %d\n", sock);
    } else if (strstr(json, "\"msg_type\":\"ota_task\"") != NULL) {
        // process OTA task (store, trigger, or print)
        printf("[GW] OTA TASK received on sock %d\n", sock);
        // send ack
        const char *ack = "{\"msg_type\":\"ota_task_ack\",\"status\":\"received\"}";
        uint32_t l = htonl(strlen(ack));
        send(sock, &l, 4, 0);
        send(sock, ack, strlen(ack), 0);
    } else if (strstr(json, "\"msg_type\":\"keep_alive_ack\"") != NULL) {
        printf("[GW] Received keep_alive_ack from %d\n", sock);
    } else {
        printf("[GW] Unknown msg type\n");
    }
}

static void *recv_task(void *arg) {
    (void)arg;
    while (1) {
        int s;
        pthread_mutex_lock(&client_lock);
        s = client_sock;
        pthread_mutex_unlock(&client_lock);

        if (s < 0) {
            // no client connected, sleep a bit
            usleep(100 * 1000);
            continue;
        }

        // read 4-byte length
        uint32_t netlen;
        int r = recv_exact(s, &netlen, 4);
        if (r <= 0) {
            printf("[GW] client %d closed or error r=%d, replacing\n", s, r);
            safe_close_client();
            continue;
        }
        uint32_t len = ntohl(netlen);
        if (len == 0 || len > MAX_PAYLOAD) {
            printf("[GW] invalid len %u, closing\n", len);
            safe_close_client();
            continue;
        }

        char *buf = malloc(len + 1);
        if (!buf) {
            printf("[GW] malloc fail\n");
            safe_close_client();
            continue;
        }
        r = recv_exact(s, buf, len);
        if (r <= 0) {
            printf("[GW] client %d closed during payload recv r=%d\n", s, r);
            free(buf);
            safe_close_client();
            continue;
        }
        buf[len] = 0;
        handle_json_message(s, buf, len);
        free(buf);
    }
    return NULL;
}

static void *accept_task(void *arg) {
    (void)arg;
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addrlen = sizeof(client_addr);
        int new_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addrlen);
        if (new_sock < 0) {
            printf("[GW] accept failed: %s\n", strerror(errno));
            usleep(200 * 1000);
            continue;
        }
        printf("[GW] Client connected, sock=%d, ip=%s, port=%d\n", new_sock,
                inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        // Set TCP_NODELAY for low-latency
        int flag = 1;
        setsockopt(new_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
        // replace client socket atomically
        set_new_client(new_sock);
    }
    return NULL;
}

int start_tcp_server() {
    server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (server_sock < 0) {
        printf("socket create failed\n");
        return -1;
    }
    int enable = 1;
    setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(SERVER_PORT);

    if (bind(server_sock, (struct sockaddr*)&addr, sizeof(addr)) != 0) {
        printf("bind failed: %s\n", strerror(errno));
        close(server_sock);
        return -1;
    }

    if (listen(server_sock, BACKLOG) != 0) {
        printf("listen failed\n");
        close(server_sock);
        return -1;
    }

    printf("[GW] TCP server listening at port %d\n", SERVER_PORT);

    // create accept and recv threads (or FreeRTOS tasks)
    pthread_t th_accept, th_recv;
    pthread_create(&th_accept, NULL, accept_task, NULL);
    pthread_create(&th_recv, NULL, recv_task, NULL);

    // detach threads if desired
    pthread_detach(th_accept);
    pthread_detach(th_recv);

    return 0;
}

/* In your main or init:
   start_tcp_server();
*/

