#include "msg_handler.h"
#include "esp_log.h"
#include "cJSON.h"
#include "client_register.h"
#include "ota_handler.h"

static const char *TAG = "GW_MSG_HANDLER";

void msg_handler_init(void) {
    ESP_LOGI(TAG, "GW Message handler initialized");
}

// 统一入口：根据 msg_type 分发到不同模块
void msg_handler_process(int sock, const char *json_str) {
    ESP_LOGI(TAG, "Processing message from sock %d: %s", sock, json_str);

    cJSON *root = cJSON_Parse(json_str);
    if (!root) {
        ESP_LOGE(TAG, "Invalid JSON: %s", json_str);
        return;
    }

    cJSON *msg_type = cJSON_GetObjectItem(root, "msg_type");
    if (cJSON_IsString(msg_type)) {
        if (strcmp(msg_type->valuestring, "register") == 0) {
            ESP_LOGI(TAG, "Dispatching to client_register");
            client_register_save(sock, root);
        } else if (strcmp(msg_type->valuestring, "progress") == 0) {
            ESP_LOGI(TAG, "Dispatching to ota_handler progress");
            ota_handler_process_progress(sock, root);
        } else if (strcmp(msg_type->valuestring, "ota_task") == 0) {
            ESP_LOGI(TAG, "Dispatching to ota_handler task");
            otaapp_process_task(sock, root);
        } else {
            ESP_LOGW(TAG, "Unknown msg_type: %s", msg_type->valuestring);
        }
    } else {
        ESP_LOGW(TAG, "JSON missing msg_type field");
    }

    cJSON_Delete(root);
}

