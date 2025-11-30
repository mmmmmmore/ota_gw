#include "webserver_otagw.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "ota_handler.h"  // 引入 ota_handler 提供的状态表接口
#include "otaapp.h"



static const char *TAG = "WEB_OTAGW";

// 全局保存当前任务信息和进度
static char current_task[512] = {0};
static bool task_pending = false;
static int current_progress = 0; // 0~100

// 接收 OTA Server 推送任务
//static esp_err_t ota_task_handler(httpd_req_t *req) {
//    int ret = httpd_req_recv(req, current_task, sizeof(current_task) - 1);
//    if (ret <= 0) {
//        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Receive error");
//        return ESP_FAIL;
//    }
//    current_task[ret] = '\0';
//    task_pending = true;
//    current_progress = 0; // 新任务进度归零
//
//    ESP_LOGI(TAG, "Received OTA task: %s", current_task);
//
//   httpd_resp_sendstr(req, "OTA task received");
//    return ESP_OK;
//}

// 提供任务信息给 UI 页面
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

    httpd_resp_sendstr(req, "Response received");
    return ESP_OK;
}




// 提供任务进度信息


// 提供所有 Client 的进度和状态信息
static esp_err_t progress_info_handler(httpd_req_t *req) {
    int count = 0;
    client_status_t *clients = ota_handler_get_status(&count);

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


// 注册路由
static void register_uri_handlers(httpd_handle_t server) {
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
