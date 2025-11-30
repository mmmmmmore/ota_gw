#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#include "esp_err.h"
#include "client_register.h"
#include "otaapp.h"

// Client 状态信息结构体（供 webserver 查询）
typedef struct {
    char client_name[32];
    int progress;          // 0~100
    int partition;         // PARTITION_A / PARTITION_B
    bool upgrading;        // 是否正在升级
    bool last_result;      // 上次结果：true=success, false=fail
} client_status_info_t;   // ← 改名，不要叫 client_status_t

// 下发任务给指定 Client ECU
esp_err_t ota_handler_send_task(const char *mac, ota_task_t *task);

// 处理来自 Client ECU 的上报消息
void ota_handler_process_message(int client_sock, const char *json_str);

// 获取所有 Client 的状态信息（供 webserver 查询）
client_status_info_t* ota_handler_get_status(int *count);

#endif // OTA_HANDLER_H
