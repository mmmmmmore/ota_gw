#include "webserver.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "ota_handler.h"

static const char *TAG = "WEB_SERVER";

// 全局保存当前任务信息
static char current_task[512] = {0};
static bool task_pending = false;

// 接收 OTA Server 推送任务
static esp_err_t ota_task_handler(httpd_req_t *req) {
    int ret = httpd_req_recv(req, current_task, sizeof(current_task) - 1);
    if (ret <= 0) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Receive error");
        return ESP_FAIL;
    }
    current_task[ret] = '\0';
    task_pending = true;

    ESP_LOGI(TAG, "Received OTA task: %s", current_task);

    httpd_resp_sendstr(req, "OTA task received");
    return ESP_OK;
}

// 提供任务信息给 HTML 页面
static esp_err_t task_info_handler(httpd_req_t *req) {
    if (task_pending) {
        httpd_resp_set_type(req, "application/json");
        httpd_resp_sendstr(req, current_task);
    } else {
        httpd_resp_sendstr(req, "{}");
    }
    return ESP_OK;
}

// 接收用户选择 Accept/Deny
static esp_err_t user_response_handler(httpd_req_t *req) {
    char buf[64];
    int ret = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (ret <= 0) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Receive error");
        return ESP_FAIL;
    }
    buf[ret] = '\0';

    ESP_LOGI(TAG, "User response: %s", buf);

    if (strcmp(buf, "Accept") == 0) {
        ESP_LOGI(TAG, "User accepted OTA task, executing...");
        ota_handler_process(current_task); // 执行任务
        task_pending = false;
    } else {
        ESP_LOGI(TAG, "User denied OTA task, ignoring.");
        task_pending = false;
    }

    httpd_resp_sendstr(req, "Response received");
    return ESP_OK;
}

// 注册路由
static void register_uri_handlers(httpd_handle_t server) {
    httpd_uri_t ota_task_uri = {
        .uri       = "/ota_task",
        .method    = HTTP_POST,
        .handler   = ota_task_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &ota_task_uri);

    httpd_uri_t task_info_uri = {
        .uri       = "/task_info",
        .method    = HTTP_GET,
        .handler   = task_info_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &task_info_uri);

    httpd_uri_t user_response_uri = {
        .uri       = "/ota_user_response",
        .method    = HTTP_POST,
        .handler   = user_response_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &user_response_uri);
}

httpd_handle_t start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        register_uri_handlers(server);
        ESP_LOGI(TAG, "Webserver started");
        return server;
    }
    ESP_LOGE(TAG, "Failed to start webserver");
    return NULL;
}
