#ifndef OTAAPP_H
#define OTAAPP_H

#include "esp_err.h"
#include "client_register.h"

// OTA 任务结构
typedef struct {
    char mac[18];
    char version[32];
    char url[128];
    char features[64]; // 可选功能集描述
} ota_task_t;

// 初始化 OTA Dispatcher
void ota_dispatch_init(void);

// 向指定 Client 下发 OTA 任务
esp_err_t ota_dispatch_send_task(const char *mac, ota_task_t *task);

// 广播 OTA 任务给所有在线 Client
esp_err_t ota_dispatch_broadcast(ota_task_t *task);

//resolve the OTA task json from tcp server
esp_err_t ota_dispatch_handle_json(const char *json_str);

//user response for the ota task
void ota_dispatch_user_response(const char *mac, ota_task_t *task, bool accepted);


//ota task mgmt
// 设置待确认任务
void otaapp_set_pending_task(ota_task_t *task);

// 获取待确认任务
ota_task_t* otaapp_get_pending_task(void);

// 清除待确认任务
void otaapp_clear_pending_task(void);

// tcp_server 的统一回调函数，由 otaapp 提供
void tcp_server_rx_handler(int client_sock, const char *data);


// ota result report interface
// 上报 Client ECU 的执行结果给 otaapp
void otaapp_report_result(const char *mac, bool success);


#endif // OTAAPP_H   //ota_dispatch




