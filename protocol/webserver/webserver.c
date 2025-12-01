#include "webserver.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "cJSON.h"
#include "ota_handler.h"
#include "otaapp.h"
#include <string.h>
#include <stdio.h>

#define PARTITION_A 0
#define PARTITION_B 1

static const char *TAG = "WEB_OTAGW";

// ---------- 静态文件处理 ----------
static esp_err_t static_file_handler(httpd_req_t *req) {
    char filepath[192] = "/spiffs";

    // 根路径返回 index.html
    if (strcmp(req->uri, "/") == 0) {
        strlcat(filepath, "/index.html", sizeof(filepath));
    } else {
        strlcat(filepath, req->uri, sizeof(filepath));
    }

    FILE *f = fopen(filepath, "rb");
    if (!f) {
        ESP_LOGW(TAG, "File not found: %s", filepath);
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File not found");
        return ESP_FAIL;
    }

    // MIME 类型
    if (strstr(filepath, ".html")) httpd_resp_set_type(req, "text/html");
    else if (strstr(filepath, ".css")) httpd_resp_set_type(req, "text/css");
    else if (strstr(filepath, ".js")) httpd_resp_set_type(req, "application/javascript");
    else if (strstr(filepath, ".png")) httpd_resp_set_type(req, "image/png");
    else if (strstr(filepath, ".ico")) httpd_resp_set_type(req, "image/x-icon");
    else httpd_resp_set_type(req, "text/plain");

    char buffer[1024];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), f)) > 0) {
        if (httpd_resp_send_chunk(req, buffer, n) != ESP_OK) {
            fclose(f);
            return ESP_FAIL;
        }
    }
    fclose(f);
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

// ---------- 任务信息 ----------
static esp_err_t task_info_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    ota_task_t *task = otaapp_get_pending_task();
    if (task) {
        cJSON *root = cJSON_CreateObject();
        cJSON_AddStringToObject(root, "version", task->version);
        cJSON_AddStringToObject(root, "url", task->url);
        cJSON_AddStringToObject(root, "features", task->features);
        char *json_str = cJSON_PrintUnformatted(root);
        httpd_resp_sendstr(req, json_str);
        free(json_str);
        cJSON_Delete(root);
    } else {
        httpd_resp_sendstr(req, "{}");
    }
    return ESP_OK;
}

// ---------- 用户响应 ----------
static esp_err_t user_response_handler(httpd_req_t *req) {
    char buf[64];
    int ret = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (ret <= 0) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Receive error");
        return ESP_FAIL;
    }
    buf[ret] = '\0';

    bool accepted = (strcmp(buf, "Accept") == 0);
    ota_task_t *task = otaapp_get_pending_task();
    if (task) {
        ota_dispatch_user_response(task->mac, task, accepted);
    }

    httpd_resp_set_type(req, "text/plain");
    httpd_resp_sendstr(req, "Response received");
    return ESP_OK;
}

// ---------- 进度信息 ----------
static esp_err_t progress_info_handler(httpd_req_t *req) {
    int count = 0;
    client_status_info_t *clients = ota_handler_get_status(&count);

    cJSON *root = cJSON_CreateArray();
    for (int i = 0; i < count; i++) {
        cJSON *item = cJSON_CreateObject();
        cJSON_AddStringToObject(item, "client", clients[i].client_name);
        cJSON_AddNumberToObject(item, "progress", clients[i].progress);
        cJSON_AddStringToObject(item, "partition",
                                clients[i].partition == PARTITION_A ? "A" : "B");
        cJSON_AddBoolToObject(item, "upgrading", clients[i].upgrading);
        cJSON_AddStringToObject(item, "result",
                                clients[i].last_result ? "success" : "fail");
        cJSON_AddItemToArray(root, item);
    }

    char *json_str = cJSON_PrintUnformatted(root);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, json_str);
    free(json_str);
    cJSON_Delete(root);
    return ESP_OK;
}

// ---------- 路由注册 ----------
static void register_uri_handlers(httpd_handle_t server) {
    // 静态资源：用通配符处理所有 GET（index、css、js、assets、favicon 等）
    httpd_uri_t static_all = {
        .uri       = "/*",
        .method    = HTTP_GET,
        .handler   = static_file_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &static_all);

    // API
    httpd_uri_t task_info_uri = {
        .uri       = "/task_info",
        .method    = HTTP_GET,
        .handler   = task_info_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &task_info_uri);

    httpd_uri_t progress_info_uri = {
        .uri       = "/progress_info",
        .method    = HTTP_GET,
        .handler   = progress_info_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &progress_info_uri);

    httpd_uri_t user_response_uri = {
        .uri       = "/ota_user_response",
        .method    = HTTP_POST,
        .handler   = user_response_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &user_response_uri);
}

// ---------- 启动/停止 ----------
httpd_handle_t start_webserver_otagw(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    // 关键：启用通配符 URI 匹配（全局）
    config.uri_match_fn = httpd_uri_match_wildcard;

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
