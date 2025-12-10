#ifndef OTAAPP_H
#define OTAAPP_H

#include "esp_err.h"
#include "cJSON.h"
#include "msg_handler.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "client_register.h"
#include "otaapp.h"



#define MAX_TASKS 6
//User response enum

typedef enum {
    OTA_STATUS_PENDING = 0,
    OTA_STATUS_ACCEPT,
    OTA_STATUS_UPDATING,
    OTA_STATUS_SUCCESS,
    OTA_STATUS_FAILED,
    OTA_STATUS_REJECTED
} ota_status_t;

typedef enum{
    USER_RESPONSE_REJECT = -1,
    USER_RESPONSE_WAIT   = 0,
    USER_RESPONSE_ACCEPT = 1
} user_response_t;


typedef enum{
    OTA_PREGRESS_IDLE = 1,
    OTA_PROGRESS_INIT,
    OTA_PROGRESS_DWLD_DONE,
    OTA_PROGRESS_UPGRADING,
    OTA_PROGRESS_COMPLETE,
} ota_progress_state_t ;


typedef struct {
    char client_id[32];
    char task_id[64];
    ota_status_t status;
    int progress; // 百分比
} ota_client_task_t;

// OTA 任务结构
typedef struct {
    char task_id[64];
    char device_name[64];
    char client_id[32];
    char version[32];
    char url[128];
    char features[64]; // 可选功能集描述
    char mac[18];
    ota_status_t status;  //ota push task ,default as pending
    uint32_t created_ms; //time stamp
    user_response_t user_response; // indicate use response from UI accept or reject.
    TimerHandle_t timer;
} ota_task_t;



typedef struct {
    ota_task_t task;
    ota_progress_state_t ota_state;
    int percentage;
    uint32_t start_ms;
    bool active; // true is active, false is inactive;
} ota_progress_t;
//---task management in otaapp -- //


void ota_dispatch_init(void);


ota_task_t* otaapp_get_task_list();
// 获取挂起任务（供 msg_handler/webserver 查询）
ota_task_t* otaapp_get_pending_task(void);

void otaapp_add_task(const ota_task_t *task); // create the task list in otaapp module


// check response from UI and webserver
void otahandler_upgrade_response(const char *task_id, user_response_t response);
// 用户响应（accept/reject） 
    // ------after user accept from UI, send task to client
        //--- send task first
esp_err_t ota_dispatch_send_task(const char *client_id, ota_task_t *task);
        //--- start update the progress
void ota_handler_on_accept(const ota_task_t *task);
        //--- listen client download finish message, and change state
void ota_handler_on_client_dwld_done(const char *task_id);
        //--- after client finish ota and reset register client,
void ota_handler_client_result_after_ota(const char *task_id);
    // ------after user reject from UI, print log 
void ota_dispatch_user_reject(const ota_task_t *task);

    // 清除挂起任务
void otaapp_clear_pending_task(void);

// after user accept, when otaapp trigger


// this function for webserver to check and call the result to display in UI.. 
const char* ota_handler_get_progress_json();



#endif // OTAAPP_H



