#include "esp_log.h"
#include "esp_http_server.h"
#include "common_gpio.h"
#include "motor_handler.h"
#include "control_mgmt.h"
#include "cJSON.h"

static const char *TAG = "webserver_motor";


// -------------------- joystick 控制处理 --------------------

esp_err_t joystick_post_handler(httpd_req_t *req) {
    char content[200];
    int ret = httpd_req_recv(req, content, sizeof(content) - 1);
    if (ret <= 0) {
        ESP_LOGE(TAG, "Failed to receive post data");
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive data");
        return ESP_FAIL;
    }

    content[ret] = '\0';
    ESP_LOGI(TAG, "Received joystick data: %s", content);

    // 解析 JSON
    cJSON *json = cJSON_Parse(content);
    if (!json) {
        ESP_LOGE(TAG, "Invalid JSON format");
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid JSON");
        return ESP_FAIL;
    }

    cJSON *uuid_item = cJSON_GetObjectItem(json, "uuid");
    cJSON *angle_item = cJSON_GetObjectItem(json, "angle");
    cJSON *distance_item = cJSON_GetObjectItem(json, "distance");

    if (!cJSON_IsString(uuid_item) || !cJSON_IsNumber(angle_item) || !cJSON_IsNumber(distance_item)) {
        ESP_LOGW(TAG, "Missing or invalid fields");
        cJSON_Delete(json);
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing or invalid fields");
        return ESP_FAIL;
    }

    const char *uuid = uuid_item->valuestring;
    int angle = angle_item->valueint;
    int distance = distance_item->valueint;

    // 验证控制权限
    if (!control_is_owner(uuid)) {
        ESP_LOGW(TAG, "Unauthorized joystick control attempt by UUID: %s", uuid);
        cJSON_Delete(json);
        httpd_resp_send_err(req, HTTPD_403_FORBIDDEN, "Not authorized");
        return ESP_FAIL;
    }

    // 执行电机控制
    motor_handler_update(angle, distance);
    cJSON_Delete(json);

    httpd_resp_sendstr(req, "OK");
    return ESP_OK;
}
// -------------------- 控制权申请处理 --------------------

static esp_err_t control_request_handler(httpd_req_t *req) {
    char content[128];
    int ret = httpd_req_recv(req, content, sizeof(content));
    if (ret <= 0) {
        ESP_LOGW(TAG, "Failed to receive request body");
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
        return ESP_FAIL;
    }

    content[ret] = '\0';

    char uuid[64] = {0};
    char *uuid_start = strstr(content, "\"uuid\":\"");
    if (uuid_start) {
        uuid_start += strlen("\"uuid\":\"");
        char *uuid_end = strchr(uuid_start, '"');
        if (uuid_end && (uuid_end - uuid_start) < sizeof(uuid)) {
            strncpy(uuid, uuid_start, uuid_end - uuid_start);
            uuid[uuid_end - uuid_start] = '\0';
        }
    }

    if (strlen(uuid) == 0) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing UUID");
        return ESP_FAIL;
    }

    bool granted = control_request(uuid);

    char response[128];
    snprintf(response, sizeof(response),
             "{ \"granted\": %s, \"current_owner\": \"%s\" }",
             granted ? "true" : "false",
             control_get_current());

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, strlen(response));
    return ESP_OK;
}

// -------------------- 控制权释放处理 --------------------

static esp_err_t control_release_handler(httpd_req_t *req) {
    char content[128];
    int ret = httpd_req_recv(req, content, sizeof(content));
    if (ret <= 0) {
        ESP_LOGW(TAG, "Failed to receive release body");
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Invalid request");
        return ESP_FAIL;
    }

    content[ret] = '\0';

    char uuid[64] = {0};
    char *uuid_start = strstr(content, "\"uuid\":\"");
    if (uuid_start) {
        uuid_start += strlen("\"uuid\":\"");
        char *uuid_end = strchr(uuid_start, '"');
        if (uuid_end && (uuid_end - uuid_start) < sizeof(uuid)) {
            strncpy(uuid, uuid_start, uuid_end - uuid_start);
            uuid[uuid_end - uuid_start] = '\0';
        }
    }

    if (strlen(uuid) == 0) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing UUID");
        return ESP_FAIL;
    }

    bool released = control_release(uuid);

    char response[128];
    snprintf(response, sizeof(response),
             "{ \"released\": %s, \"new_owner\": \"%s\" }",
             released ? "true" : "false",
             control_get_current());

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, response, strlen(response));
    return ESP_OK;
}

// -------------------- URI 注册 --------------------

esp_err_t webserver_motor_register(httpd_handle_t server) {
    control_manager_init();

    httpd_uri_t joystick_uri = {
        .uri       = "/joystick",
        .method    = HTTP_POST,
        .handler   = joystick_post_handler,
        .user_ctx  = NULL
    };

    httpd_uri_t control_request_uri = {
        .uri       = "/control/request",
        .method    = HTTP_POST,
        .handler   = control_request_handler,
        .user_ctx  = NULL
    };

    httpd_uri_t control_release_uri = {
        .uri       = "/control/release",
        .method    = HTTP_POST,
        .handler   = control_release_handler,
        .user_ctx  = NULL
    };

    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &joystick_uri));
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &control_request_uri));
    ESP_ERROR_CHECK(httpd_register_uri_handler(server, &control_release_uri));

    ESP_LOGI(TAG, "Motor control URIs registered");
    return ESP_OK;
}
