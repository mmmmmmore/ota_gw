#include "webserver.h"
#include "esp_log.h"
#include "esp_http_client.h"

static const char *TAG = "WEB_SERVER";

// OTA 结果接收接口（供 GW 内部调用）
static esp_err_t ota_result_handler(httpd_req_t *req) {
    char buf[256];
    int ret = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (ret <= 0) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Receive error");
        return ESP_FAIL;
    }
    buf[ret] = '\0';

    ESP_LOGI(TAG, "Received OTA result from client: %s", buf);

    // 将结果转发给 OTA Server
    esp_http_client_config_t config = {
        .url = "http://<OTA_SERVER_IP>:8080/ota_result", // OTA Server 接收结果的地址
        .method = HTTP_METHOD_POST,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_header(client, "Content-Type", "application/json");
    esp_http_client_set_post_field(client, buf, strlen(buf));

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Result forwarded to OTA Server, status = %d",
                 esp_http_client_get_status_code(client));
    } else {
        ESP_LOGE(TAG, "Failed to forward result: %s", esp_err_to_name(err));
    }

    esp_http_client_cleanup(client);

    httpd_resp_sendstr(req, "Result received and forwarded");
    return ESP_OK;
}

// 注册 URI 路由
static void register_uri_handlers(httpd_handle_t server) {
    httpd_uri_t ota_task_uri = {
        .uri       = "/ota_task",
        .method    = HTTP_POST,
        .handler   = ota_post_handler, // 已有的任务接收函数
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &ota_task_uri);

    httpd_uri_t ota_result_uri = {
        .uri       = "/ota_result",
        .method    = HTTP_POST,
        .handler   = ota_result_handler, // 新增的结果转发函数
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &ota_result_uri);
}
