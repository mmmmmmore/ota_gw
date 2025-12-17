#include "otaapp.h"
#include "tcp_server.h"
#include "client_register.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>




static const char *TAG = "OTA_APP_MGMT";
static ota_task_t pending_task;   // 唯一的挂起任务缓存
static ota_task_t task_lists[MAX_TASKS] ;
// 全局保存 Client 状态表


static int client_count = 0;
static ota_progress_t ota_handler_progress;     
static TimerHandle_t ota_handler_upgrade_timer = NULL;

void ota_task_timeout_cb(TimerHandle_t xTimer) {
    ota_task_t *task = (ota_task_t*) pvTimerGetTimerID(xTimer);
    //Second Change_2, release memory resource
    if (task->timer){
        xTimerStop(task->timer, 0);
        xTimerDelete(task->timer, 0);
        task->timer =NULL;
    }
    memset(task, 0, sizeof(*task));
}

void otaapp_add_task(const ota_task_t *task) {
    // 找到空槽位
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_lists[i].task_id[0] == '\0') {
            task_lists[i] = *task;
            task_lists[i].user_response = USER_RESPONSE_WAIT;

            // 创建定时器，60s 后触发
            task_lists[i].timer = xTimerCreate("TaskTimer", pdMS_TO_TICKS(600000),
                                              pdFALSE, (void*)&task_lists[i],
                                              ota_task_timeout_cb);
            ESP_LOGI(TAG, "New Task [%s] created in Queue, wait for User response ", task_lists[i].task_id);
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
        memcpy(send_buf, json_str, len);
        send_buf[len] = '\n';
        send_buf[len+1] = '\0';
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

// after user accept, when otaapp trigger
void ota_handler_on_accept(const char *task_id){
    // setup the change
    strncpy(ota_handler_progress.task_id_sts, task_id, sizeof(ota_handler_progress.task_id_sts)-1);
    ota_handler_progress.task_id_sts[sizeof(ota_handler_progress.task_id_sts)-1] = '\0';
    
    ota_handler_progress.ota_state = OTA_PROGRESS_INIT;
    ota_handler_progress.percentage = 5 ;
    ota_handler_progress.start_ms = esp_log_timestamp();
    ota_handler_progress.active = true;
    ESP_LOGI(TAG, " [%s] init OTA progress for client = ", ota_handler_progress.task_id_sts );
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


// inside Webserver after UI select choise in page, will trigger this function
// 
void otahandler_upgrade_response(const char *task_id, user_response_t response) {
    for (int i = 0; i < MAX_TASKS; i++) {
        if (strcmp(task_lists[i].task_id, task_id) == 0) {
            task_lists[i].user_response = response;
            if (response == USER_RESPONSE_ACCEPT) {
                //trigger the process update function in ota_handler
                ota_dispatch_send_task(task_lists[i].client_id, &task_lists[i]);
                ota_handler_on_accept(task_id);
            } else if (response == USER_RESPONSE_REJECT){
                ESP_LOGI(TAG, "Task %s rejected", task_id);
                ota_dispatch_user_reject( &task_lists[i]);
            } else {
                ESP_LOGI(TAG, "Task %s waiting ", &task_lists[i].task_id);
            }

            // 停止定时器并清理任务   change_1
            if (task_lists[i].timer){
                xTimerStop(task_lists[i].timer, 0);
                xTimerDelete(task_lists[i].timer, 0);
                task_lists[i].timer = NULL;
            }
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



void ota_handler_on_client_dwld_done(const char *task_id){
    ESP_LOGI(TAG, "OTA module Rx Client send download finish signal");
    if (strcmp(task_id, ota_handler_progress.task_id_sts) == 0){
        ota_handler_progress.ota_state = OTA_PROGRESS_DWLD_DONE;
        ota_handler_progress.percentage = 15;
        ESP_LOGI(TAG, "OTA progress update to %d percentage ", ota_handler_progress.percentage);
        ota_handler_progress.start_ms = esp_log_timestamp(); 
        // trigger auto clock 
        if (!ota_handler_upgrade_timer) {
            ESP_LOGI(TAG, "OTA under progress, update the percentage");
            ota_handler_upgrade_timer = xTimerCreate("ota_upgrade_time",
                                                    pdMS_TO_TICKS(1000),
                                                    pdTRUE,
                                                    NULL,
                                                    ota_handler_upgrade_timer_cb);
            xTimerStart(ota_handler_upgrade_timer, 0);
        }
    }
}



void ota_handler_client_result_after_ota(const char *task_id ){
    if (strcmp(task_id, ota_handler_progress.task_id_sts) == 0 ){
        ESP_LOGI(TAG, "Rx client report ota task [%s] after reset. setup the as complete", task_id);
        ota_handler_progress.ota_state = OTA_PROGRESS_COMPLETE;
        ota_handler_progress.percentage = 100 ;
        ota_handler_progress.active = false ;
        if (ota_handler_upgrade_timer){
            xTimerStop(ota_handler_upgrade_timer, 0);
            xTimerDelete(ota_handler_upgrade_timer, 0);
            ota_handler_upgrade_timer = NULL;
        }
        memset(&ota_handler_progress, 0, sizeof(ota_handler_progress));  //release all finished resource.
    }

}


static void ota_cleanup_progress(void){
    if (ota_handler_upgrade_timer) {
        xTimerStop(ota_handler_upgrade_timer, 0);
        xTimerDelete(ota_handler_upgrade_timer, 0);
        ota_handler_upgrade_timer = NULL;
    }
    memset(&ota_handler_upgrade_timer, 0, sizeof(ota_handler_upgrade_timer));
}

static void ota_cleanup_task_entry(ota_task_t *t){
    if(!t) return;
    if(t->timer){
        xTimerStop(t->timer, 0);
        xTimerDelete(t->timer, 0);
        t->timer =NULL;
    }
    memset(t, 0, sizeof(*t));
}

static void ota_on_terminal_state(const char *task_id, ota_progress_t term){
    //cleanup all task
    ota_cleanup_progress();

    //clean by task list table
    for (int i =0; i<MAX_TASKS; i++){
        if(task_lists[i].task_id[0] && strcmp(task_lists[i].task_id, task_id)== 0){
            ota_cleanup_task_entry(&task_lists[i]);
            break;
        }
    }
}

// this function for webserver to check and call the result to display in UI.. 
size_t ota_handler_get_progress_json(char *buf,  size_t buflen){
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "task_id", ota_handler_progress.task_id_sts);
    cJSON_AddNumberToObject(root, "progress", ota_handler_progress.percentage);
    const char *state=
            (ota_handler_progress.ota_state == OTA_PROGRESS_INIT ) ? "init" :
            (ota_handler_progress.ota_state == OTA_PROGRESS_DWLD_DONE ) ? "dwld_done" :
            (ota_handler_progress.ota_state == OTA_PROGRESS_UPGRADING ) ? "upgrading" :
            (ota_handler_progress.ota_state == OTA_PROGRESS_COMPLETE ) ? "complete" : "idle";
    cJSON_AddStringToObject(root, "state", state);
    char *json_str = cJSON_PrintUnformatted(root);
    size_t len = json_str? strlen(json_str) :0 ;
    if (json_str && len < buflen) {
        memcpy(buf, json_str, len+1);
    }else if ( buflen > 0){
        buf[0] = '\0';
    }

    cJSON_Delete(root);
    if (json_str) free(json_str);
    return len;  // call user use and free this memory

}







