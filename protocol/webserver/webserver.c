#include "webserver.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "cJSON.h"
#include "msg_handler.h"   // 替换 otaapp/ota_handler

// ---------- 待确认任务信息 ----------
static esp_err_t task_info_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    const char *json_str = msg_handler_get_pending_task_json();
    httpd_resp_sendstr(req, json_str ? json_str : "{}");
    if (json_str) free((void*)json_str);
    return ESP_OK;
}

// ---------- 用户响应 ----------
static esp_err_t user_response_handler(httpd_req_t *req) {
    char buf[256];
    int ret = httpd_req_recv(req, buf, sizeof(buf) - 1);
    if (ret <= 0) {
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Receive error");
        return ESP_FAIL;
    }
    buf[ret] = '\0';

    cJSON *root = cJSON_Parse(buf);
    if (!root) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
        return ESP_FAIL;
    }

    cJSON *decision_item = cJSON_GetObjectItem(root, "decision");
    cJSON *client_id_item = cJSON_GetObjectItem(root, "client_id");

    if (!cJSON_IsString(decision_item) || !cJSON_IsString(client_id_item)) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing fields");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    bool accepted = (strcmp(decision_item->valuestring, "accept") == 0);
    msg_handler_user_response(client_id_item->valuestring, accepted);

    cJSON_Delete(root);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, "{\"status\":\"ok\"}");
    return ESP_OK;
}

// ---------- 进度信息 ----------
static esp_err_t progress_info_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    const char *json_str = msg_handler_get_progress_json();
    httpd_resp_sendstr(req, json_str ? json_str : "[]");
    if (json_str) free((void*)json_str);
    return ESP_OK;
}
