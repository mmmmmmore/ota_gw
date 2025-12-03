#ifndef OTAAPP_H
#define OTAAPP_H

#include "esp_err.h"
#include "client_register.h"
#include "cJSON.h"

// OTA 任务结构
typedef struct {
    char task_id[64];
    char device_name[64];
    char client_id[32];
    char version[32];
    char url[128];
    char features[64]; // 可选功能集描述
    char mac[18];
} ota_task_t;


// 清除挂起任务
void otaapp_clear_pending_task(void);

// 设置挂起任务（由 msg_handler 调用）
void otaapp_set_pending_task(const ota_task_t *task);

// 获取挂起任务（供 msg_handler/webserver 查询）
ota_task_t* otaapp_get_pending_task(void);

// 用户响应（accept/reject）
void ota_dispatch_user_response(const char *client_id, ota_task_t *task, bool accepted);

// 下发任务给指定客户端
esp_err_t ota_dispatch_send_task(const char *client_id, ota_task_t *task);

#endif // OTAAPP_H



