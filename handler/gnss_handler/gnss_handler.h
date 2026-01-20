#ifndef GNSS_HANDLER_H
#define GNSS_HANDLER_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#pragma pack(1)
// NMEA GPGGA 数据结构
typedef struct {
    char time[10];          // UTC 时间 (hhmmss.ss)
    char latitude[12];      // 纬度
    char ns_indicator;      // N/S 指示
    char longitude[13];     // 经度
    char ew_indicator;      // E/W 指示
    char fix_quality[2];    // 定位质量 (0-8)
    char num_satellites[3]; // 卫星数量
    char hdop[6];          // 水平精度因子
    char altitude[10];      // 海拔高度
    char altitude_unit;     // 高度单位 (M)
    char timestamp_ms;      // 毫秒戳 (更新时间)
    bool valid;            // 数据有效标志
} gnss_gpgga_data_t;
#pragma pack()

// 初始化 GNSS 处理
void gnss_handler_init(void);

// 启动 GNSS 数据接收任务
void gnss_handler_start_task(void);

// 停止 GNSS 数据接收任务
void gnss_handler_stop_task(void);

// 获取最新的 GPGGA 数据
bool gnss_handler_get_latest_gpgga(gnss_gpgga_data_t *data);

// 获取最新的 GPGGA 数据为 JSON 字符串
char* gnss_handler_get_gpgga_json(void);

// 时间同步 - 从 GPGGA 数据更新系统时间
bool gnss_handler_sync_system_time(void);

// 获取时间校准状态
bool gnss_handler_get_time_synced(void);

// 获取最后同步时间戳
uint32_t gnss_handler_get_last_sync_timestamp(void);

#endif // GNSS_HANDLER_H
