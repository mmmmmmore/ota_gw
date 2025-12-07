#ifndef OTAAPP_H
#define OTAAPP_H

#include "esp_err.h"
#include "client_register.h"
#include "cJSON.h"
#include "msg_handler.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_err.h"




#define MAX_TASKS 6
//User response enum

typedef enum{
    USER_RESPONSE_REJECT = -1,
    USER_RESPONSE_WAIT   = 0,
    USER_RESPONSE_ACCEPT = 1
} user_response_t;


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


//---task management in otaapp -- //

void otaapp_add_task(const ota_task_t *task);

ota_task_t* otaapp_get_task_list();

// 清除挂起任务
void otaapp_clear_pending_task(void);

// 获取挂起任务（供 msg_handler/webserver 查询）
ota_task_t* otaapp_get_pending_task(void);



void otaapp_update_response(const char *task_id, user_response_t response);

// 设置挂起任务（由 msg_handler 调用） // old single case, disable in task list mode
//void otaapp_set_pending_task(const ota_task_t *task);



void ota_dispatch_init(void);
// 用户响应（accept/reject）
void ota_dispatch_user_reject(const ota_task_t *task);

// 下发任务给指定客户端
esp_err_t ota_dispatch_send_task(const char *client_id, ota_task_t *task);

#endif // OTAAPP_H



