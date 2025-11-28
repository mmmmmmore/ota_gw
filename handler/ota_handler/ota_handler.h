#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#include <stdbool.h>

// Client 分区状态
typedef enum {
    PARTITION_A,
    PARTITION_B
} client_partition_t;

// Client 状态表
typedef struct {
    char client_name[32];
    client_partition_t partition;
    bool upgrading;
    int progress; // 0~100
    bool last_result; // true=成功, false=失败
} client_status_t;

// 初始化 OTA Handler
void ota_handler_init(void);

// 处理来自 Webserver 的 OTA 通知
void ota_handler_process(const char *task_json);

// 模拟进度更新（供定时器或任务调用）
void ota_handler_update_progress(void);

// 获取 Client 状态表（供 Webserver 查询）
client_status_t *ota_handler_get_status(int *count);

#endif // OTA_HANDLER_H
