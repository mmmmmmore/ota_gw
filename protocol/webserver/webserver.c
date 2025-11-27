#include "webserver.h"
#include "esp_log.h"
#include "ota_handler.h"

static const char *TAG = "WEB_SERVER";

// OTA POST 请求处理函数
static esp_err_t ota_post_handler(httpd_req_t *req) {
    char buf[512];
    int ret = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (ret <= 0) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Receive error");
        return ESP_FAIL;
    }

    buf[ret] = '\0'; // 确保字符串结束
    ESP_LOGI(TAG, "Received OTA task: %s", buf);

    // 调用 ota_handler 处理任务
    ota_handler_process(buf);

    httpd_resp_sendstr(req, "OTA task received");
    return ESP_OK;
}

// 注册 URI 路由
static void register_uri_handlers(httpd_handle_t server) {
    httpd_uri_t ota_task_uri = {
        .uri       = "/ota_task",
        .method    = HTTP_POST,
        .handler   = ota_post_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &ota_task_uri);
}

// 启动 WebServer
httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 8080; // 可自定义端口

    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        register_uri_handlers(server);
        ESP_LOGI(TAG, "Webserver started on port %d", config.server_port);
        return server;
    }

    ESP_LOGE(TAG, "Failed to start webserver");
    return NULL;
}

// 停止 WebServer
void stop_webserver(httpd_handle_t server) {
    if (server) {
        httpd_stop(server);
        ESP_LOGI(TAG, "Webserver stopped");
    }
}
