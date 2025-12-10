#include "webserver.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "cJSON.h"
#include "msg_handler.h"   // 替换 otaapp/ota_handler
#include <stdio.h>
#include <string.h>
#include "otaapp.h"

static const char *TAG = "WEB_OTAGW";
//static char *pending_task_json = NULL;




// ---------- 静态文件处理 ----------
static esp_err_t static_file_handler(httpd_req_t *req) {
    char filepath[192] = "/spiffs";

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

//-----------valid task display in UI page ---------//
char *webserver_get_task_list_json(void) {
    cJSON *root = cJSON_CreateArray();
    ota_task_t *task_lists = otaapp_get_task_list();
    for (int i = 0; i < MAX_TASKS; i++) {
        ota_task_t *t = &task_lists[i];
        if (t->task_id[0] == '\0') continue; // 空槽位跳过

        // 检查是否过期
        if (esp_log_timestamp() - t->created_ms > 60000) continue;

        cJSON *obj = cJSON_CreateObject();
        cJSON_AddStringToObject(obj, "task_id", t->task_id);
        cJSON_AddStringToObject(obj, "client_id", t->client_id);
        cJSON_AddStringToObject(obj, "version", t->version);
        cJSON_AddStringToObject(obj, "features", t->features);
        cJSON_AddNumberToObject(obj, "user_response", t->user_response);

        cJSON_AddItemToArray(root, obj);
    }

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str; // 调用者负责 free()
}

//const char *msg_handler_get_pending_task_json(void) {
//    return pending_task_json ? strdup(pending_task_json) : NULL;
//}



// ---------- 待确认任务信息 ----------
static esp_err_t task_info_handler(httpd_req_t *req) {
    httpd_resp_set_type(req, "application/json");
    char *json_str = webserver_get_task_list_json();
    httpd_resp_sendstr(req, json_str ? json_str : "{}");
    if (json_str) free(json_str);
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

    cJSON *response_item = cJSON_GetObjectItem(root, "response");
    cJSON *task_id_item = cJSON_GetObjectItem(root, "task_id");

    if (!cJSON_IsString(response_item) || !cJSON_IsString(task_id_item)) {
        httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Missing fields");
        cJSON_Delete(root);
        return ESP_FAIL;
    }

    user_response_t user_response_value ;
    if (strcmp(response_item->valuestring, "accept") == 0) {
        user_response_value = USER_RESPONSE_ACCEPT;
    } else if (strcmp(response_item->valuestring, "reject") == 0) {
        user_response_value = USER_RESPONSE_REJECT;
    } else {
        user_response_value = USER_RESPONSE_WAIT; // 默认兜底
    }    // matching UI response with ota_task_t --> user_response value. 
    // after webserver receive the UI accept / reject choice, then send to ota_handler execution
    otahandler_upgrade_response(task_id_item->valuestring, user_response_value);  
    ESP_LOGI(TAG, "User have response to task: %s", task_id_item->valuestring);
    cJSON_Delete(root);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, "{\"status\":\"ok\"}");
    return ESP_OK;
}

// ---------- 进度信息 ----------
static esp_err_t progress_info_handler(httpd_req_t *req) {
    char *json = ota_handler_get_progress_json();
    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, json, strlen(json));
    free(json);
    return ESP_OK;
}




// ---------- 路由注册 ----------
static void register_uri_handlers(httpd_handle_t server) {
    // API 路由
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

    // 静态资源通配符最后注册
    httpd_uri_t static_all = {
        .uri       = "/*",
        .method    = HTTP_GET,
        .handler   = static_file_handler,
        .user_ctx  = NULL
    };
    httpd_register_uri_handler(server, &static_all);
}

// ---------- 启动/停止 ----------
httpd_handle_t start_webserver_otagw(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.uri_match_fn = httpd_uri_match_wildcard;
    config.max_open_sockets =10;  //set up max to 8 sockets

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
