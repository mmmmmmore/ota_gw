#include "webserver_otagw.h"
#include "esp_log.h"
#include "esp_http_client.h"

static const char *TAG = "WEB_OTAGW";

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

// 提供任务信息给 UI 页面
static esp_err_t task_info_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    if (task_pending) {
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
        ESP_LOGI(TAG, "User accepted OTA task, notify ota_handler...");
        // TODO: 调用 ota_handler_process(current_task);
        task_pending = false;
    } else {
        ESP_LOGI(TAG, "User denied OTA task, reporting to OTA Server...");
        // 将拒绝结果转发给 OTA Server
        esp_http_client_config_t config = {
            .url = "http://<OTA_SERVER_IP>:8080/ota_result",
            .method = HTTP_METHOD_POST,
        };
        esp_http_client_handle_t client = esp_http_client_init(&config);
        esp_http_client_set_header(client, "Content-Type", "application/json");
        const char *deny_msg = "{\"result\":\"deny\"}";
        esp_http_client_set_post_field(client, deny_msg, strlen(deny_msg));
        esp_http_client_perform(client);
        esp_http_client_cleanup(client);

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

httpd_handle_t start_webserver_otagw(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK) {
        register_uri_handlers(server);
        ESP_LOGI(TAG, "OTA Gateway Webserver started");
        return server;
    }
    ESP_LOGE(TAG, "Failed to start OTA Gateway Webserver");
    return NULL;
}

void stop_webserver_otagw(httpd_handle_t server) {
    if (server) {
        httpd_stop(server);
        ESP_LOGI(TAG, "OTA Gateway Webserver stopped");
    }
}
