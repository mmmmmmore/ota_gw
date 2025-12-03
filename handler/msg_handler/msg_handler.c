#include "msg_handler.h"
#include "esp_log.h"
#include "cJSON.h"
#include "client_register.h"
#include "ota_handler.h"
#include "otaapp.h"

static const char *TAG = "GW_MSG_HANDLER";

void msg_handler_init(void) {
    ESP_LOGI(TAG, "GW Message handler initialized");
}

void msg_handler_process(int sock, const char *json_str, msg_role_t role) {
    ESP_LOGI(TAG, "Processing message from sock %d (role=%d): %s", sock, role, json_str);

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
            ESP_LOGI(TAG, "Dispatching to otaapp task");

            // 构造 ota_task_t
            ota_task_t task;
            memset(&task, 0, sizeof(task));

            cJSON *task_id   = cJSON_GetObjectItem(root, "task_id");
            cJSON *dev_name  = cJSON_GetObjectItem(root, "device_name");
            cJSON *client_id = cJSON_GetObjectItem(root, "client_id");
            cJSON *version   = cJSON_GetObjectItem(root, "version");
            cJSON *url       = cJSON_GetObjectItem(root, "firmware_url");
            cJSON *features  = cJSON_GetObjectItem(root, "features");

            if (cJSON_IsString(task_id))   strncpy(task.task_id, task_id->valuestring, sizeof(task.task_id)-1);
            if (cJSON_IsString(dev_name))  strncpy(task.device_name, dev_name->valuestring, sizeof(task.device_name)-1);
            if (cJSON_IsString(client_id)) strncpy(task.client_id, client_id->valuestring, sizeof(task.client_id)-1);
            if (cJSON_IsString(version))   strncpy(task.version, version->valuestring, sizeof(task.version)-1);
            if (cJSON_IsString(url))       strncpy(task.url, url->valuestring, sizeof(task.url)-1);
            if (cJSON_IsString(features))  strncpy(task.features, features->valuestring, sizeof(task.features)-1);

            // 用 client_id 来推送任务
            if (task.client_id[0] != '\0') {
                ota_dispatch_send_task(task.client_id, &task);
            } else {
                ESP_LOGW(TAG, "OTA task missing client_id, cannot dispatch");
            }

        } else {
            ESP_LOGW(TAG, "Unknown msg_type: %s", msg_type->valuestring);
        }
    } else {
        ESP_LOGW(TAG, "JSON missing msg_type field");
    }

    cJSON_Delete(root);
}
