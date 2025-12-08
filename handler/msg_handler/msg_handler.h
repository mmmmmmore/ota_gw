#ifndef MSG_HANDLER_H
#define MSG_HANDLER_H

#include "cJSON.h"
#include "esp_err.h"

typedef enum {
    ROLE_UNKNOWN = 0,
    ROLE_OTA_SERVER,
    ROLE_CLIENT
} msg_role_t;

typedef enum {
    OTA_STATUS_PENDING = 0,
    OTA_STATUS_ACCEPT,
    OTA_STATUS_UPDATING,
    OTA_STATUS_SUCCESS,
    OTA_STATUS_FAILED,
    OTA_STATUS_REJECTED
} ota_status_t;

typedef struct {
    char client_id[32];
    char task_id[64];
    ota_status_t status;
    int progress; // 百分比
} ota_client_task_t;



// 初始化
void msg_handler_init(void);

// 消息处理入口
void msg_handler_process(int sock, const char *json_str, msg_role_t role);

// 提供给 webserver 的接口
//const char* msg_handler_get_pending_task_json(void);   // 返回待确认任务 JSON
void msg_handler_user_response(const char *client_id, bool accepted); // 用户响应
const char* msg_handler_get_progress_json(void);       // 返回进度 JSON

#endif // MSG_HANDLER_H
