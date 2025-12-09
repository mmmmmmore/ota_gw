#include "otaapp.h"
#include "tcp_server.h"
#include "client_register.h"

#include "esp_log.h"
#include "cJSON.h"
#include <string.h>
#include "esp_log.h"



static const char *TAG = "OTA_APP_MGMT";
static ota_task_t pending_task;   // 唯一的挂起任务缓存
static ota_task_t task_lists[MAX_TASKS] ;


void ota_task_timeout_cb(TimerHandle_t xTimer) {
    ota_task_t *task = (ota_task_t*) pvTimerGetTimerID(xTimer);
    if (task->user_response == USER_RESPONSE_WAIT) {
        ESP_LOGW(TAG, "Task %s timed out", task->task_id);
        memset(task, 0, sizeof(*task));
    }
}

void otaapp_add_task(const ota_task_t *task) {
    // 找到空槽位
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_lists[i].task_id[0] == '\0') {
            task_lists[i] = *task;
            task_lists[i].user_response = USER_RESPONSE_WAIT;

            // 创建定时器，60s 后触发
            task_lists[i].timer = xTimerCreate("TaskTimer", pdMS_TO_TICKS(60000),
                                              pdFALSE, (void*)&task_lists[i],
                                              ota_task_timeout_cb);
            xTimerStart(task_lists[i].timer, 0);

            // 发给 Webserver 用于 UI 展示
            //webserver_notify_new_task(&task_lists[i]);
            return;
        }
    }
    ESP_LOGW(TAG, "Task list full, discard new task");
}





// 清除挂起任务
void otaapp_clear_pending_task(void) {
    memset(&pending_task, 0, sizeof(pending_task));
    ESP_LOGI(TAG, "Pending task cleared");
}

// 设置挂起任务
//void otaapp_set_pending_task(const ota_task_t *task) {
//    ota_task_t *slot = find_slot_by_client_id(task->client_id);     //link to pending_tasks_list 
//    if (slot) {
//        *slot = *task;
//        slot->user_response = USER_RESPONSE_WAIT;    // default setup as Wait for User decision. 
//        pending_task = *task; // 结构体拷贝
//        ESP_LOGI(TAG, "Pending task stored: id=%s, client=%s, version=%s",
//                 pending_task.task_id, pending_task.client_id, pending_task.version);
//    }
//}

void ota_dispatch_init(void) {
    otaapp_clear_pending_task();
}


// 获取挂起任务
ota_task_t* otaapp_get_pending_task(void) {
    return (pending_task.task_id[0] != '\0') ? &pending_task : NULL;
}



// 用户响应
void ota_dispatch_user_reject(const ota_task_t *task ) {

    ESP_LOGW(TAG, "User rejected OTA task %s ", task->task_id);

        // 构造反馈 JSON 给 OTA Server
    cJSON *root = cJSON_CreateObject();        
    cJSON_AddStringToObject(root, "msg_type", "ota_task_ack");
    cJSON_AddStringToObject(root, "task_id", task->task_id);
    cJSON_AddStringToObject(root, "status", "rejected");
    cJSON_AddStringToObject(root, "version", task->version);
    char *json_str = cJSON_PrintUnformatted(root);

    size_t len = strlen(json_str);
    char *send_buf = malloc(len + 2); 
    if (send_buf) {
        strcpy(send_buf, json_str);
        strcpy(send_buf, "\n");
    }

    int ota_sock = tcp_server_get_ota_sock();
    if (ota_sock >= 0) {
        ESP_LOGI(TAG, "Sending reject result to OTA Server: %s", send_buf);
        tcp_server_send(ota_sock, send_buf);
    } else {
        ESP_LOGW(TAG, "No OTA Server socket available, cannot send reject result");
    }
    free(send_buf);
    cJSON_Delete(root);
    free(json_str);
    
}

static const char* user_response_to_str(user_response_t resp){
    switch (resp)
    {
        case USER_RESPONSE_ACCEPT: return "User_Accept";
        case USER_RESPONSE_REJECT: return "User_Reject";
        case USER_RESPONSE_WAIT:   return "User_Wait";
        default: return "User_Wait";
    }
}



// 全局保存 Client 状态表
static client_status_info_t client_status_list[MAX_CLIENTS];
static int client_count = 0;
static ota_progress_t ota_handler_progress;     
static TimerHandle_t ota_handler_upgrade_time = NULL;











void otahandler_upgrade_response(const char *task_id, user_response_t response) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (strcmp(task_lists[i].task_id, task_id) == 0) {
            task_lists[i].user_response = response;
            if (response == USER_RESPONSE_ACCEPT) {
                //trigger the process update function in ota_handler
                ota_dispatch_send_task(task_lists[i].client_id, &task_lists[i]);
            } else if (response == USER_RESPONSE_REJECT){
                ESP_LOGI(TAG, "Task %s rejected", task_id);
                ota_dispatch_user_reject(&task_lists[i]);
            } else {
                ESP_LOGI(TAG, "Task %s waiting ", &task_lists[i].task_id);
            }

            // 停止定时器并清理任务
            xTimerStop(task_lists[i].timer, 0);
            memset(&task_lists[i], 0, sizeof(task_lists[i]));
            return;
        }
    }
    ESP_LOGW(TAG, "Task %s not found for update", task_id);
}

ota_task_t* otaapp_get_task_list(void){
    return task_lists;
}

// 下发任务给指定客户端
esp_err_t ota_dispatch_send_task(const char *client_id, ota_task_t *task) {
    client_info_t *client = client_register_find_by_client_id(client_id);
    if (!client || client->state == CLIENT_OFFLINE) {
        ESP_LOGW(TAG, "Client %s not found or offline", client_id);
        return ESP_FAIL;
    }

    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "msg_type", "ota_task");
    cJSON_AddStringToObject(root, "task_id", task->task_id);
    cJSON_AddStringToObject(root, "device_name", task->device_name);
    cJSON_AddStringToObject(root, "client_id", task->client_id);
    cJSON_AddStringToObject(root, "version", task->version);
    cJSON_AddStringToObject(root, "firmware_url", task->url);
    cJSON_AddStringToObject(root, "features", task->features);
    cJSON_AddStringToObject(root, "user_response", user_response_to_str(task->user_response));

    char *json_str = cJSON_PrintUnformatted(root);
    ESP_LOGI(TAG, "Sending OTA task to client %s (IP=%s): %s,  UI response: %s", client_id, client->ip, json_str, user_response_to_str(task->user_response));

    esp_err_t ret = tcp_server_send(client->sock, json_str);

    cJSON_Delete(root);
    free(json_str);

    if (ret == ESP_OK) {
        client->state = CLIENT_UPDATING;
        ESP_LOGI(TAG, "Client %s state updated to UPDATING", client_id);
    } else {
        ESP_LOGE(TAG, "Failed to send OTA task to client %s", client_id);
    }
    return ret;
}

//-----------clock tick the progress ---------------//
static void ota_handler_upgrade_timer_cb(TimerHandle_t xTimer){
    if (ota_handler_progress.ota_state == OTA_PROGRESS_DWLD_DONE ||
        ota_handler_progress.ota_state == OTA_PROGRESS_UPGRADING){
            ota_handler_progress.ota_state = OTA_PROGRESS_UPGRADING;
            uint32_t elapsed = esp_log_timestamp() - ota_handler_progress.start_ms ;
            int pct = 15 + (elapsed / 1000) * 5;  // +5 / s
            if(pct > 90) pct = 90;
            if (pct > ota_handler_progress.percentage){
                ota_handler_progress.percentage = pct ;
                ESP_LOGI(TAG, " progress upgrading ::: %d%% ", ota_handler_progress.percentage);
            }
        }
}


// after user accept, when otaapp trigger
void ota_handler_on_accept(const ota_task_t *task){
    // setup the change
    ota_handler_progress.task= *task;
    ota_handler_progress.ota_state = OTA_PROGRESS_INIT;
    ota_handler_progress.percentage = 5 ;
    ota_handler_progress.start_ms = esp_log_timestamp();
    ota_handler_progress.active = true;
    ESP_LOGI(TAG, " [%s] init OTA progress for client = %s", ota_handler_progress.task.task_id, ota_handler_progress.task.client_id);
}



void ota_handler_on_client_dwld_done(const char *task_id){
    if (strcmp(task_id, ota_handler_progress.task.task_id) == 0){
        ota_handler_progress.ota_state = OTA_PROGRESS_DWLD_DONE;
        ota_handler_progress.percentage = 15;
        ota_handler_progress.start_ms = esp_log_timestamp(); 
        // trigger auto clock 
        if (!ota_handler_upgrade_time) {
            ota_handler_upgrade_time = xTimerCreate("ota_upgrade_time",
                                                    pdMS_TO_TICKS(1000),
                                                    pdTRUE,
                                                    NULL,
                                                    ota_handler_upgrade_time_cb);
            xTimerStart(ota_handler_upgrade_time, 0);
        }
    }
}



void ota_handler_on_client_register(const char *client_id){
    if (strcmp(client_id, ota_handler_progress.task.client_id) == 0){
        ota_handler_progress.ota_state = OTA_PROGRESS_COMPLETE;
        ota_handler_progress.percentage = 100 ;
        ota_handler_progress.active = false ;
        if (ota_handler_upgrade_time){
            xTimerStop(ota_handler_upgrade_timer, 0);
            xTimerDelete(ota_handler_upgrade_timer, 0);
            ota_handler_upgrade_timer = NULL;
        }
    }
}




// this function for webserver to check and call the result to display in UI.. 
const char* ota_handler_get_progress_json(){
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "task_id", ota_handler_progress.task.task_id);
    cJSON_AddStringToObject(root, "client_id", ota_handler_progress.task.client_id);
    cJSON_AddNumberToObject(root, "progress", ota_handler_progress.percentage);

    const char *state=
            (ota_handler_progress.ota_state == OTA_PROGRESS_INIT ) ? "init" :
            (ota_handler_progress.ota_state == OTA_PROGRESS_DWLD_DONE ) ? "dwld_done" :
            (ota_handler_progress.ota_state == OTA_PROGRESS_UPGRADING ) ? "upgrading" :
            (ota_handler_progress.ota_state == OTA_PROGRESS_COMPLETE ) ? "complete" : "idle";
    cJSON_AddStringToObject(root, "state", state);

    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    return json_str;  // call user use and free this memory

}







