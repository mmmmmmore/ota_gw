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

// 初始化 OTA Dispatcher
void ota_dispatch_init(void);

// 向指定 Client 下发 OTA 任务
esp_err_t ota_dispatch_send_task(const char *mac, ota_task_t *task);

// 广播 OTA 任务给所有在线 Client
esp_err_t ota_dispatch_broadcast(ota_task_t *task);

// 解析并保存 OTA Server 下发的任务 JSON
esp_err_t ota_dispatch_handle_json(const char *json_str);

// 用户响应 OTA 任务
void ota_dispatch_user_response(const char *mac, ota_task_t *task, bool accepted);

// OTA 任务管理
void otaapp_set_pending_task(ota_task_t *task);
ota_task_t* otaapp_get_pending_task(void);
void otaapp_clear_pending_task(void);

// 新增：由 msg_handler 调用，处理 OTA Server 下发的任务
void otaapp_process_task(int sock, cJSON *root);

// 上报 Client ECU 的执行结果给 OTA Server
void otaapp_report_result(const char *mac, bool success);

#endif // OTAAPP_H
