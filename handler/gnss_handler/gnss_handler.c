#include "gnss_handler.h"
#include "common_gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_system.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
#include "cJSON.h"

static const char *TAG = "GNSS_HANDLER";

#define NMEA_BUFFER_SIZE 256
#define GNSS_QUEUE_SIZE 10

// 全局变量
static gnss_gpgga_data_t g_latest_gpgga = {0};
static bool g_time_synced = false;
static uint32_t g_last_sync_timestamp = 0;
static QueueHandle_t gnss_queue = NULL;
static TaskHandle_t gnss_task_handle = NULL;
static int uart_error_count = 0; // counts consecutive UART read errors

// NMEA 校验和计算
static uint8_t nmea_checksum(const char *sentence) {
    uint8_t checksum = 0;
    const char *start = strchr(sentence, '$');
    const char *end = strchr(sentence, '*');
    
    if (!start || !end) return 0;
    
    start++; // 跳过 $
    while (start < end) {
        checksum ^= *start;
        start++;
    }
    return checksum;
}

// 验证 NMEA 校验和
static bool nmea_verify_checksum(const char *sentence) {
    const char *checksum_str = strchr(sentence, '*');
    if (!checksum_str) return false;
    
    uint8_t expected = nmea_checksum(sentence);
    uint8_t actual = (uint8_t)strtol(checksum_str + 1, NULL, 16);
    
    return expected == actual;
}

// 解析 GPGGA 句子
static bool parse_gpgga_sentence(const char *sentence, gnss_gpgga_data_t *data) {
    if (!data) return false;
    
    // 检查格式
    if (strncmp(sentence, "$GPGGA", 6) != 0) {
        return false;
    }
    
    // 验证校验和
    if (!nmea_verify_checksum(sentence)) {
        ESP_LOGW(TAG, "GPGGA checksum error");
        return false;
    }
    
    // 复制原始句子用于解析
    char temp[NMEA_BUFFER_SIZE];
    strncpy(temp, sentence, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    // 移除 $ 和 *checksum
    char *end = strchr(temp, '*');
    if (end) *end = '\0';
    
    char *p = strchr(temp, ',');
    if (!p) return false;
    p++; // 跳过第一个逗号
    
    // 解析 UTC 时间
    end = strchr(p, ',');
    if (!end) return false;
    strncpy(data->time, p, (end - p < sizeof(data->time) ? end - p : sizeof(data->time) - 1));
    data->time[end - p] = '\0';
    
    // 解析纬度
    p = end + 1;
    end = strchr(p, ',');
    if (!end) return false;
    strncpy(data->latitude, p, (end - p < sizeof(data->latitude) ? end - p : sizeof(data->latitude) - 1));
    data->latitude[end - p] = '\0';
    
    // 解析 N/S 指示
    p = end + 1;
    if (*p) data->ns_indicator = *p;
    p += 2; // 跳过指示符和逗号
    
    // 解析经度
    end = strchr(p, ',');
    if (!end) return false;
    strncpy(data->longitude, p, (end - p < sizeof(data->longitude) ? end - p : sizeof(data->longitude) - 1));
    data->longitude[end - p] = '\0';
    
    // 解析 E/W 指示
    p = end + 1;
    if (*p) data->ew_indicator = *p;
    p += 2;
    
    // 解析定位质量
    end = strchr(p, ',');
    if (!end) return false;
    strncpy(data->fix_quality, p, (end - p < sizeof(data->fix_quality) ? end - p : sizeof(data->fix_quality) - 1));
    data->fix_quality[end - p] = '\0';
    
    // 解析卫星数量
    p = end + 1;
    end = strchr(p, ',');
    if (!end) return false;
    strncpy(data->num_satellites, p, (end - p < sizeof(data->num_satellites) ? end - p : sizeof(data->num_satellites) - 1));
    data->num_satellites[end - p] = '\0';
    
    // 解析 HDOP
    p = end + 1;
    end = strchr(p, ',');
    if (!end) return false;
    strncpy(data->hdop, p, (end - p < sizeof(data->hdop) ? end - p : sizeof(data->hdop) - 1));
    data->hdop[end - p] = '\0';
    
    // 解析高度
    p = end + 1;
    end = strchr(p, ',');
    if (!end) return false;
    strncpy(data->altitude, p, (end - p < sizeof(data->altitude) ? end - p : sizeof(data->altitude) - 1));
    data->altitude[end - p] = '\0';
    
    // 解析高度单位
    p = end + 1;
    if (*p) data->altitude_unit = *p;
    
    data->valid = (atoi(data->fix_quality) > 0);
    data->timestamp_ms = (uint8_t)(esp_log_timestamp() & 0xFF);
    
    return true;
}

// GNSS 数据接收任务
static void gnss_receive_task(void *pvParameter) {
    char data[NMEA_BUFFER_SIZE];
    int data_len = 0;
    gnss_gpgga_data_t parsed_data;
    
    ESP_LOGI(TAG, "GNSS receive task started");
    
    while (1) {
        // If driver not installed, back off to 1 Hz logging and skip reads
        if (!uart_is_driver_installed(UART_UBLOX_PORT)) {
            static uint32_t last_log = 0;
            uint32_t now = esp_log_timestamp();
            if ((now - last_log) >= 1000) {
                ESP_LOGW(TAG, "UART driver not installed yet (skipping reads)");
                last_log = now;
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        // 从 UART 读取数据
        int len = uart_read_bytes(UART_UBLOX_PORT, (uint8_t *)&data[data_len],
                                   NMEA_BUFFER_SIZE - data_len - 1, 200 / portTICK_PERIOD_MS);
        
        // Diagnostic: Log UART status every 10 reads
        static int read_attempts = 0;
        read_attempts++;
        if (read_attempts % 10 == 0) {
            ESP_LOGI(TAG, "UART status: read_attempt=%d, len=%d, buffer=%d bytes", 
                     read_attempts, len, data_len);
        }
        
        if (len > 0) {
            uart_error_count = 0; // reset error counter on success
            ESP_LOGI(TAG, "UART RX: %d bytes received", len);
            
            // Log raw data (first 64 bytes) to verify data format
            char hex_str[196];
            int print_len = (len < 64) ? len : 64;
            for (int i = 0; i < print_len && i < (int)sizeof(hex_str)/3; i++) {
                sprintf(&hex_str[i*3], "%02X ", (uint8_t)data[data_len + i]);
            }
            ESP_LOGI(TAG, "Raw data: %s", hex_str);
            
            data_len += len;
            
            // 查找完整的 NMEA 句子 (以 \r\n 结尾)
            char *end = strstr(data, "\r\n");
            if (end) {
                *end = '\0';
                
                // 只处理 GPGGA 句子
                if (strstr(data, "$GPGGA") != NULL) {
                    if (parse_gpgga_sentence(data, &parsed_data)) {
                        memcpy(&g_latest_gpgga, &parsed_data, sizeof(gnss_gpgga_data_t));
                        
                        if (g_latest_gpgga.valid) {
                            ESP_LOGI(TAG, "GPGGA parsed: time=%s, lat=%s%c, lon=%s%c, sats=%s",
                                    g_latest_gpgga.time, g_latest_gpgga.latitude, 
                                    g_latest_gpgga.ns_indicator, g_latest_gpgga.longitude,
                                    g_latest_gpgga.ew_indicator, g_latest_gpgga.num_satellites);
                        } else {
                            ESP_LOGW(TAG, "GPGGA data invalid (fix quality=%s)", 
                                    g_latest_gpgga.fix_quality);
                        }
                    }
                }
                
                // 移除已处理的数据
                data_len -= (end - data + 2);
                if (data_len > 0) {
                    memmove(data, end + 2, data_len);
                }
            }
        } else if (len < 0) {
            // UART error occurred, log it at ~5 Hz max (actually every 5s)
            static uint32_t last_error_log = 0;
            uint32_t now = esp_log_timestamp();
            uart_error_count++;

            if ((now - last_error_log) > 5000) {
                uart_port_t port = UART_UBLOX_PORT;
                uint32_t baud = 0;
                bool installed = uart_is_driver_installed(port);

                if (installed && uart_get_baudrate(port, &baud) == ESP_OK) {
                    uart_word_length_t wlen;
                    uart_parity_t parity;
                    uart_stop_bits_t stop_bits;
                    uart_hw_flowcontrol_t flow;

                    uart_get_word_length(port, &wlen);
                    uart_get_parity(port, &parity);
                    uart_get_stop_bits(port, &stop_bits);
                    uart_get_hw_flow_ctrl(port, &flow);

                    ESP_LOGW(TAG, "UART read error (len=%d) count=%d baud=%" PRIu32 " data_bits=%d parity=%d stop=%d flow=%d", 
                             len, uart_error_count, baud, wlen, parity, stop_bits, flow);
                } else {
                    ESP_LOGW(TAG, "UART read error (len=%d) count=%d (driver installed=%d, baud read err)",
                             len, uart_error_count, installed);
                }

                // Drain RX FIFO to clear potential junk
                uint8_t dump_buf[32];
                int drained = uart_read_bytes(port, dump_buf, sizeof(dump_buf), 10 / portTICK_PERIOD_MS);
                if (drained > 0) {
                    ESP_LOGW(TAG, "Drained %d bytes from RX FIFO after error", drained);
                }

                last_error_log = now;
            }
        }
        
        // Slow the GNSS poll rate to ~1 Hz to reduce UART traffic/noise
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

// ===== 公开接口 =====

void gnss_handler_init(void) {
    ESP_LOGI(TAG, "Initializing GNSS handler");
    
    // UART Ublox is already initialized in common_gpio_init()
    // No need to call uart_ublox_init() again
    
    // 创建队列
    gnss_queue = xQueueCreate(GNSS_QUEUE_SIZE, sizeof(gnss_gpgga_data_t));
    if (gnss_queue == NULL) {
        ESP_LOGE(TAG, "Failed to create GNSS queue");
        return;
    }
    
    // Initialize global variables with default values
    memset(&g_latest_gpgga, 0, sizeof(gnss_gpgga_data_t));
    strcpy(g_latest_gpgga.time, "000000.00");           // Default: 00:00:00.00
    strcpy(g_latest_gpgga.latitude, "0.0000");
    strcpy(g_latest_gpgga.longitude, "0.0000");
    g_latest_gpgga.ns_indicator = 'N';
    g_latest_gpgga.ew_indicator = 'E';
    strcpy(g_latest_gpgga.fix_quality, "0");            // 0 = No fix
    strcpy(g_latest_gpgga.num_satellites, "0");
    strcpy(g_latest_gpgga.hdop, "0.0");
    strcpy(g_latest_gpgga.altitude, "0.0");
    g_latest_gpgga.altitude_unit = 'M';
    g_latest_gpgga.valid = false;                         // No valid data until first fix
    g_time_synced = false;
    g_last_sync_timestamp = 0;
    
    ESP_LOGI(TAG, "GNSS handler initialized with default values");
}

void gnss_handler_start_task(void) {
    if (gnss_task_handle != NULL) {
        ESP_LOGW(TAG, "GNSS task already started");
        return;
    }
    
    if (xTaskCreate(gnss_receive_task, "gnss_rx", 4096, NULL, 5, &gnss_task_handle) != pdPASS) {
        ESP_LOGE(TAG, "Failed to create GNSS receive task");
        return;
    }
    
    ESP_LOGI(TAG, "GNSS receive task started");
}

void gnss_handler_stop_task(void) {
    if (gnss_task_handle != NULL) {
        vTaskDelete(gnss_task_handle);
        gnss_task_handle = NULL;
        ESP_LOGI(TAG, "GNSS receive task stopped");
    }
}

bool gnss_handler_get_latest_gpgga(gnss_gpgga_data_t *data) {
    if (data == NULL) return false;
    
    memcpy(data, &g_latest_gpgga, sizeof(gnss_gpgga_data_t));
    return g_latest_gpgga.valid;
}

char* gnss_handler_get_gpgga_json(void) {
    cJSON *root = cJSON_CreateObject();
    
    // Add data fields
    cJSON_AddStringToObject(root, "time", g_latest_gpgga.time);
    cJSON_AddStringToObject(root, "latitude", g_latest_gpgga.latitude);
    cJSON_AddNumberToObject(root, "ns_indicator", g_latest_gpgga.ns_indicator);
    cJSON_AddStringToObject(root, "longitude", g_latest_gpgga.longitude);
    cJSON_AddNumberToObject(root, "ew_indicator", g_latest_gpgga.ew_indicator);
    cJSON_AddStringToObject(root, "fix_quality", g_latest_gpgga.fix_quality);
    cJSON_AddStringToObject(root, "num_satellites", g_latest_gpgga.num_satellites);
    cJSON_AddStringToObject(root, "hdop", g_latest_gpgga.hdop);
    cJSON_AddStringToObject(root, "altitude", g_latest_gpgga.altitude);
    cJSON_AddNumberToObject(root, "altitude_unit", g_latest_gpgga.altitude_unit);
    cJSON_AddBoolToObject(root, "valid", g_latest_gpgga.valid);
    cJSON_AddBoolToObject(root, "time_synced", g_time_synced);
    
    // Add status message
    if (!g_latest_gpgga.valid) {
        cJSON_AddStringToObject(root, "status", "NO_SIGNAL");
        cJSON_AddStringToObject(root, "message", "GNSS module has no valid signal. Using default values (2020-01-01 00:00:00)");
    } else {
        cJSON_AddStringToObject(root, "status", "FIX_OK");
        cJSON_AddStringToObject(root, "message", "GNSS data is valid and ready for time synchronization");
    }
    
    char *json_str = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    
    return json_str;
}

bool gnss_handler_sync_system_time(void) {
    if (!g_latest_gpgga.valid) {
        ESP_LOGW(TAG, "Cannot sync time - GPGGA data not valid");
        return false;
    }
    
    // 解析 UTC 时间 (hhmmss.ss)
    struct tm time_info = {0};
    int hours, minutes, seconds, milliseconds;
    
    // 简单解析 hhmmss.ss
    if (sscanf(g_latest_gpgga.time, "%2d%2d%2d.%d", 
               &hours, &minutes, &seconds, &milliseconds) != 4) {
        ESP_LOGE(TAG, "Failed to parse time from GPGGA");
        return false;
    }
    
    time_info.tm_hour = hours;
    time_info.tm_min = minutes;
    time_info.tm_sec = seconds;
    time_info.tm_year = 70; // 默认 1970 年
    time_info.tm_mon = 0;   // 默认 1 月
    time_info.tm_mday = 1;  // 默认 1 日
    
    time_t now = mktime(&time_info);
    
    // 设置系统时间
    struct timeval tv = {
        .tv_sec = now,
        .tv_usec = milliseconds * 1000
    };
    
    if (settimeofday(&tv, NULL) == 0) {
        g_time_synced = true;
        g_last_sync_timestamp = esp_log_timestamp();
        ESP_LOGI(TAG, "System time synced from GNSS: %02d:%02d:%02d", 
                hours, minutes, seconds);
        return true;
    } else {
        ESP_LOGE(TAG, "Failed to set system time");
        return false;
    }
}

bool gnss_handler_get_time_synced(void) {
    return g_time_synced;
}

uint32_t gnss_handler_get_last_sync_timestamp(void) {
    return g_last_sync_timestamp;
}
