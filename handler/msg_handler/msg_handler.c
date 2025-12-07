#include "msg_handler.h"
#include "esp_log.h"
#include "cJSON.h"
#include "client_register.h"
#include "otaapp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "freertos/task.h"

static const char *TAG = "GW_MSG_HANDLER";

#define MAX_CLIENT_TASKS 16
static ota_client_task_t client_task_status[MAX_CLIENT_TASKS];

void msg_handler_init(void) {
    memset(client_task_status, 0, sizeof(client_task_status));
    ESP_LOGI(TAG, "GW Message handler initialized");
}

static ota_client_task_t* find_or_create_task(const char *client_id) {
    for (int i = 0; i < MAX_CLIENT_TASKS; i++) {
        if (client_task_status[i].client_id[0] &&
            strcmp(client_task_status[i].client_id, client_id) == 0) {
            return &client_task_status[i];
        }
    }
    for (int i = 0; i < MAX_CLIENT_TASKS; i++) {
        if (client_task_status[i].client_id[0] == '\0') {
            strncpy(client_task_status[i].client_id, client_id,
                    sizeof(client_task_status[i].client_id)-1);
            return &client_task_status[i];
        }
    }
    return NULL;
}

static void delayed_send_task(void *param) {
    ota_task_t *task = (ota_task_t *) param;
    vTaskDelay(pdMS_TO_TICKS(10000));
    ESP_LOGI(TAG, "Delayed dispatch: sending task %s to client %s",
             task->task_id, task->client_id);
    ota_dispatch_send_task(task->client_id, task);
    // vPortFree(task);
    vTaskDelete(NULL);
}
// 在 msg_handler_process() 的 ota_task 分支

////

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
            client_register_save(sock, root);
        } else if (strcmp(msg_type->valuestring, "progress") == 0) {
            const char *client_id = cJSON_GetObjectItem(root,"client_id")->valuestring;
            ota_client_task_t *task = find_or_create_task(client_id);
            if (task) {
                task->progress = cJSON_GetObjectItem(root,"percent")->valueint;
                task->status = OTA_STATUS_UPDATING;
            }

        } else if (strcmp(msg_type->valuestring, "ota_task") == 0) {
            ESP_LOGI(TAG, "GW Rx ota task, schedule delay dispatch to otaapp ");
            // 构造 ota_task_t
            ota_task_t *task = pvPortMalloc(sizeof(ota_task_t));
            if (!task){
                ESP_LOGE(TAG, "Fail to allocate task");
                cJSON_Delete(root);
                return;
            }
            memset(task, 0, sizeof(ota_task_t));
            //construct task msg
            cJSON *task_id   = cJSON_GetObjectItem(root, "task_id");
            cJSON *dev_name  = cJSON_GetObjectItem(root, "device_name");
            cJSON *client_id = cJSON_GetObjectItem(root, "client_id");
            cJSON *version   = cJSON_GetObjectItem(root, "version");
            cJSON *url       = cJSON_GetObjectItem(root, "firmware_url");
            cJSON *features  = cJSON_GetObjectItem(root, "features");
        
            
            if (cJSON_IsString(task_id))   strncpy(task->task_id, task_id->valuestring, sizeof(task->task_id)-1);
            if (cJSON_IsString(dev_name))  strncpy(task->device_name, dev_name->valuestring, sizeof(task->device_name)-1);
            if (cJSON_IsString(client_id)) strncpy(task->client_id, client_id->valuestring, sizeof(task->client_id)-1);
            if (cJSON_IsString(version))   strncpy(task->version, version->valuestring, sizeof(task->version)-1);
            if (cJSON_IsString(url))       strncpy(task->url, url->valuestring, sizeof(task->url)-1);
            if (cJSON_IsString(features))  strncpy(task->features, features->valuestring, sizeof(task->features)-1);

            task->status = OTA_STATUS_PENDING;          //default set pending
            task->created_ms = esp_log_timestamp();     //default set time
            task->user_response = USER_RESPONSE_WAIT;   //default set wait

            
            // 将任务存储到 otaapp 的 pending_task
            otaapp_add_task(task);
            
            ota_client_task_t *status = find_or_create_task(task->client_id);
            if (status){
                strncpy(status->task_id, task->task_id, sizeof(status->task_id)-1);
                status->status = OTA_STATUS_PENDING;
                status->progress = 0;
            }
            //delay the task send 10s
            xTaskCreate(delayed_send_task, "DelayedSendTask", 4096, task, 5, NULL);
            


            // 用 client_id 来推送任务
            //if (task.client_id[0] != '\0') {
            //    ota_dispatch_send_task(task.client_id, &task);
            //} else {
            //    ESP_LOGW(TAG, "OTA task missing client_id, cannot dispatch");
            //}

        } else if (strcmp(msg_type->valuestring, "result") == 0) {
            cJSON *cid = cJSON_GetObjectItem(root,"client_id");
            cJSON *status = cJSON_GetObjectItem(root,"status");
            if (cJSON_IsString(cid) && cJSON_IsString(status)) {
                ota_client_task_t *task = find_or_create_task(cid->valuestring);
                if (task) {
                    task->status = (strcmp(status->valuestring,"success")==0) ? OTA_STATUS_SUCCESS : OTA_STATUS_FAILED;
                    task->progress = 100;
                }
            }   
        }
    }

    cJSON_Delete(root);
}

// ---------- 提供给 webserver 的接口 ----------

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

//void msg_handler_user_response(const char *client_id, bool accepted) {
//   ota_task_t *task = otaapp_get_pending_task();
//    if (task && strcmp(task->client_id, client_id) == 0 && task->status == OTA_STATUS_PENDING) {
//        if (accepted) {
//            ota_dispatch_user_response(client_id, task, true);
//        } else {
//            task->status = OTA_STATUS_REJECTED;
//            ESP_LOGW(TAG, "Task %s rejected by web", task->task_id);
//        }
//    } else {
//        ESP_LOGW(TAG, "No matching pending task for client %s", client_id);
//    }/
//}


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
                case OTA_STATUS_ACCEPT: status_str = "accept"; break;
                case OTA_STATUS_REJECTED: status_str = "rejected"; break;
            }
            cJSON_AddStringToObject(item, "status", status_str);
            cJSON_AddItemToArray(root, item);
        }
    }
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str;
}






