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

const char* msg_handler_get_pending_task_json(void) {
    ota_task_t *task = otaapp_get_pending_task();
    if (!task) return NULL;
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "task_id", task->task_id);
    cJSON_AddStringToObject(root, "device_name", task->device_name);
    cJSON_AddStringToObject(root, "client_id", task->client_id);
    cJSON_AddStringToObject(root, "version", task->version);
    cJSON_AddStringToObject(root, "url", task->url);
    cJSON_AddStringToObject(root, "features", task->features);
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str;
}


void msg_handler_user_response(const char *client_id, bool accepted) {
    ota_task_t *task = otaapp_get_pending_task();
    if (task) {
        ota_dispatch_user_response(client_id, task, accepted);
    } else {
        ESP_LOGW(TAG, "No pending task to respond");
    }
}


const char* msg_handler_get_progress_json(void) {
    cJSON *root = cJSON_CreateArray();
    for (int i = 0; i < MAX_CLIENT_TASKS; i++) {
        if (client_task_status[i].client_id[0]) {
            cJSON *item = cJSON_CreateObject();
            cJSON_AddStringToObject(item, "client_id", client_task_status[i].client_id);
            cJSON_AddStringToObject(item, "task_id", client_task_status[i].task_id);
            cJSON_AddNumberToObject(item, "progress", client_task_status[i].progress);
            const char *status_str = "unknown";
            switch (client_task_status[i].status) {
                case OTA_STATUS_PENDING: status_str = "pending"; break;
                case OTA_STATUS_UPDATING: status_str = "updating"; break;
                case OTA_STATUS_SUCCESS: status_str = "success"; break;
                case OTA_STATUS_FAILED: status_str = "failed"; break;
            }
            cJSON_AddStringToObject(item, "status", status_str);
            cJSON_AddItemToArray(root, item);
        }
    }
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str;
}


