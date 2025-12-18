#include "otaapp.h"
#include "tcp_server.h"
#include "client_register.h"
#include "esp_log.h"
#include "cJSON.h"
#include <string.h>




static const char *TAG = "OTA_APP_MGMT";

static ota_task_t task_lists[MAX_TASKS] ;




void ota_task_timeout_cb(TimerHandle_t xTimer) {
    ota_task_t *task = (ota_task_t*) pvTimerGetTimerID(xTimer);
    //Second Change_2, release memory resource
    if (!task) return;
    if (task->active == true){
        xTimerChangePeriod(task->life_timer, pdMS_TO_TICKS(15000), 0);
        return;
    }else {
        ESP_LOGI(TAG, "Task : %s need force cleanup", task->task_id);
    }
    if(task->upgrade_timer){
        xTimerStop(task->upgrade_timer, 0);
        xTimerDelete(task->upgrade_timer, 0);
        task->upgrade_timer = NULL;
    }

    if (task->life_timer){
        xTimerStop(task->life_timer, 0);
        xTimerDelete(task->life_timer, 0);
        task->life_timer =NULL;
    }
    memset(task, 0, sizeof(*task));
}

void otaapp_add_task( ota_task_t *task) {
    // variant transfer not use const
    // 找到空槽位
    for (int i = 0; i < MAX_TASKS; i++) {
        if (task_lists[i].task_id[0] == '\0') {
            task_lists[i] = *task;
            task_lists[i].user_response = USER_RESPONSE_WAIT;
            // 创建定时器，600s 后触发
            task_lists[i].life_timer = xTimerCreate("TaskTimer", pdMS_TO_TICKS(600000),
                                              pdFALSE, (void*)&task_lists[i],
                                              ota_task_timeout_cb);
            ESP_LOGI(TAG, "New Task [%s] created in Queue, wait for User response ", task_lists[i].task_id);
            xTimerStart(task_lists[i].life_timer, 0);
            return;
        }
    }
    ESP_LOGW(TAG, "Task list full, discard new task");
}



// 用户响应
void ota_dispatch_user_reject( ota_task_t *task ) {

    ESP_LOGW(TAG, "User rejected OTA task %s ", task->task_id);
    task->active = false;
    task->status = OTA_STATUS_REJECTED;
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

// after user accept, when otaapp trigger, only execution task will maintain the struct.
void ota_handler_on_accept(ota_task_t *task){
    // setup the change, eliminate the invalid task before task dispatch;
    ota_task_valid_check();
    //-----setup new task progress
    task->ota_progress_state = OTA_PROGRESS_INIT;
    task->percentage = 5 ;
    task->created_ms = esp_log_timestamp();
    task->active = true;
    ESP_LOGI(TAG, " [%s] init OTA progress for client = %s", task->task_id, task->client_id);  

    ota_dispatch_send_task(task);
    // after send out , wait for the dwld_done from client
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
                ota_handler_on_accept(&task_lists[i]);
                //dispatch action inside the on accpet
            } else if (response == USER_RESPONSE_REJECT){
                ESP_LOGI(TAG, "Task %s rejected", task_id);
                ota_dispatch_user_reject( &task_lists[i]);
            } else {
                ESP_LOGI(TAG, "Task %s waiting ", task_lists[i].task_id);
            }
        }
    }
    ESP_LOGW(TAG, "Task %s not found for update", task_id);
}



ota_task_t* otaapp_get_task_list(void){
    return task_lists;
}

// 下发任务给指定客户端
esp_err_t ota_dispatch_send_task(ota_task_t *task) {
    client_info_t *client = client_register_find_by_client_id(task->client_id);
    if (!client || client->state == CLIENT_OFFLINE) {
        ESP_LOGW(TAG, "Client %s not found or offline", task->client_id);
        return ESP_FAIL;
    }
    //construct the task json to client
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
    ESP_LOGI(TAG, "Sending OTA task to client %s (IP=%s): %s,  UI response: %s", task->client_id, client->ip, json_str, user_response_to_str(task->user_response));

    esp_err_t ret = tcp_server_send(client->sock, json_str);

    cJSON_Delete(root);
    free(json_str);

    if (ret == ESP_OK) {
        client->state = CLIENT_UPDATING;
        ESP_LOGI(TAG, "Client %s state updated to UPDATING", task->client_id);
    } else {
        ESP_LOGE(TAG, "Failed to send OTA task to client %s", task->client_id);
    }
    return ret;
}


//-----------clock tick the progress ---------------//
static void ota_handler_upgrade_timer_cb(TimerHandle_t xTimer){
    ota_task_t *task = (ota_task_t*) pvTimerGetTimerID(xTimer);
    if (task->ota_progress_state == OTA_PROGRESS_DWLD_DONE ||
        task->ota_progress_state == OTA_PROGRESS_UPGRADING){
            task->ota_progress_state = OTA_PROGRESS_UPGRADING;
            uint32_t elapsed = esp_log_timestamp() - task->created_ms ;
            int pct = 15 + (elapsed / 1000) * 5;  // +5 / s
            if(pct > 90) pct = 90;
            if (pct > task->percentage){
                task->percentage = pct ;
                ESP_LOGI(TAG, " progress upgrading :: %d%% ", task->percentage);
            }
        }
}

//from client trigger this change
void ota_handler_on_client_dwld_done(const char *task_id){
    ESP_LOGI(TAG, "OTA module Rx Client send download finish signal");
    for (int i =0; i< MAX_TASKS; i++){
        if (strcmp(task_id, task_lists[i].task_id) == 0){
            task_lists[i].percentage = 15;
            task_lists[i].ota_progress_state = OTA_PROGRESS_DWLD_DONE;
            task_lists[i].created_ms = esp_log_timestamp();
            if (!task_lists[i].upgrade_timer) {
                task_lists[i].upgrade_timer= xTimerCreate("ota_upgrade_time", pdMS_TO_TICKS(1000), pdTRUE, (void*)&task_lists[i], ota_handler_upgrade_timer_cb);
                xTimerStart(task_lists[i].upgrade_timer,0);
            }
        }
    }
}


void ota_handler_client_result_after_ota(const char *task_id ){
    for (int i =0; i<MAX_TASKS; i++){
        if (strcmp(task_id, task_lists[i].task_id) == 0){
            ESP_LOGI(TAG, "Rx client [%s] tx ota task [%s], result complete", task_lists[i].client_id, task_lists[i].task_id);

            task_lists[i].percentage = 100;
            task_lists[i].active = false;
            task_lists[i].ota_progress_state = OTA_PROGRESS_COMPLETE;
            task_lists[i].status = OTA_STATUS_SUCCESS;
            xTimerStop(task_lists[i].upgrade_timer,0);
            xTimerDelete(task_lists[i].upgrade_timer,0);
            //need cleanup the resource afterwards before next task;
        }
    }
}


static void ota_task_valid_check(){
    for (int i=0; i < MAX_TASKS; i++){
        if (task_lists[i].status == OTA_STATUS_SUCCESS ||
            task_lists[i].percentage == 100 ||
            task_lists[i].active == false){
                ota_cleanup_task_entry(&task_lists[i]);
            }
    }
}


static void ota_cleanup_task_entry(ota_task_t *t){
    if(!t) return;
    if(t->life_timer){
        xTimerStop(t->life_timer, 0);
        xTimerDelete(t->life_timer, 0);
        t->life_timer =NULL;
    }
    if(t->upgrade_timer){
        xTimerStop(t->upgrade_timer, 0);
        xTimerDelete(t->upgrade_timer, 0);
        t->upgrade_timer = NULL;
    }
    memset(t, 0, sizeof(*t));
}



// this function for webserver to check and call the result to display in UI.. 
size_t ota_handler_get_progress_json(char *buf,  size_t buflen){
    size_t len =0;
    for (int i =0; i< MAX_TASKS; i++){
        if (task_lists[i].ota_progress_state != OTA_PROGRESS_IDLE){
            cJSON *root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "task_id", task_lists[i].task_id);
            cJSON_AddNumberToObject(root, "progress", task_lists[i].percentage);
            const char *state=
                (task_lists[i].ota_progress_state == OTA_PROGRESS_INIT ) ? "init" :
                (task_lists[i].ota_progress_state == OTA_PROGRESS_DWLD_DONE ) ? "dwld_done" :
                (task_lists[i].ota_progress_state == OTA_PROGRESS_UPGRADING ) ? "upgrading" :
                (task_lists[i].ota_progress_state == OTA_PROGRESS_COMPLETE ) ? "complete" : "idle";
            
            cJSON_AddStringToObject(root, "state", state);
            char *json_str = cJSON_PrintUnformatted(root);
            len = json_str? strlen(json_str) :0 ;
            if (json_str && len < buflen) {
                memcpy(buf, json_str, len+1);
            }else if ( buflen > 0){
                buf[0] = '\0';
            }
            cJSON_Delete(root);
            if (json_str) free(json_str);
        }
    }
    return len;  // 
}







