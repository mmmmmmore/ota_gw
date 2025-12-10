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


//-------reverse here, will disable in next version patch--------//
static ota_client_task_t* find_or_create_task(const char *task_id) {
    for (int i = 0; i < MAX_CLIENT_TASKS; i++) {
        if (client_task_status[i].task_id[0] &&
            strcmp(client_task_status[i].task_id, task_id) == 0) {
            return &client_task_status[i];
        }
    }
    for (int i = 0; i < MAX_CLIENT_TASKS; i++) {
        if (client_task_status[i].task_id[0] == '\0') {
            strncpy(client_task_status[i].task_id, task_id,
                    sizeof(client_task_status[i].task_id)-1);
            return &client_task_status[i];
        }
    }
    return NULL;
}
///-- this is only for pending 10s test ---//
//static void delayed_send_task(void *param) {
//    ota_task_t *task = (ota_task_t *) param;
//    vTaskDelay(pdMS_TO_TICKS(10000));
//    ESP_LOGI(TAG, "Delayed dispatch: sending task %s to client %s",
//             task->task_id, task->client_id);
//    ota_dispatch_send_task(task->client_id, task);
//    // vPortFree(task);
//    vTaskDelete(NULL);
//}
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
            //
            //ota_handler_check_result();
        } else if(strcmp(msg_type->valuestring, "hello") == 0){
            ESP_LOGI(TAG, "OTA server hello received on sock %d", sock);
            //tsp_server_set_ota_sock(sock);
        } else if (strcmp(msg_type->valuestring, "ota_progress") == 0) {
            /// how to resolve this ota_progress message
            cJSON *task_id_item = cJSON_GetObjectItem(root, "task_id");
            const char *task_id = cJSON_IsString(task_id_item)? task_id_item->valuestring : NULL;
            //const char *client_id = cJSON_GetObjectItem(root, "client_id")->valuestring;
            //const char *ota_state = cJSON_GetObjectItem(root, "state")->valuestring;

            ota_handler_on_client_dwld_done(task_id); // transfer the task id and ota state to ota handler
            ESP_LOGI(TAG, "msg_handler tx the ota_progress to otaapp finished");
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
            // task trasnfer to otaapp , manage by otaapp, include the task list. 

        } else if (strcmp(msg_type->valuestring, "ota_result") == 0) {
            cJSON *task_id_item = cJSON_GetObjectItem(root,"task_id");
            const char *task_id = cJSON_IsString(task_id_item)? task_id_item->valuestring:NULL;
            ota_handler_client_result_after_ota(task_id); 
            //cJSON *ota_state = cJSON_GetObjectItem(root,"state")->valuestring;
            //valid ota task result have task_id, invalid task id is Null
        } else{
            ESP_LOGI(TAG, "non known json data rxed.");
        }
    }

    cJSON_Delete(root);
}

// ---------- 提供给 webserver 的接口 ----------
//const char* msg_handler_get_pending_task_json(void) {
//    ota_task_t *task = otaapp_get_pending_task();
//    if (!task) return NULL;
//    cJSON *root = cJSON_CreateObject();
//    cJSON_AddStringToObject(root, "task_id", task->task_id);
//    cJSON_AddStringToObject(root, "device_name", task->device_name);
//    cJSON_AddStringToObject(root, "client_id", task->client_id);
//    cJSON_AddStringToObject(root, "version", task->version);
//    cJSON_AddStringToObject(root, "url", task->url);
//    cJSON_AddStringToObject(root, "features", task->features);
//    char *json_str = cJSON_PrintUnformatted(root);
//    cJSON_Delete(root);
//    return json_str;
//}
/// ----------251209 disable this function, since all task status was transfer to otaapp realize and maintain. 
/// ----------this function callback in webserver also disable. 



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






