#include <stdio.h>
#include "wifi_ap.h"
#include "webserver.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_spiffs.h"
#include "common_gpio.h"
#include "init.h"
#include "esp_psram.h"
#include "esp_heap_caps.h"
#include "ota_handler.h"



#include <dirent.h>   // 用于目录遍历
#include "esp_log.h"

void init_spiffs(){
    esp_vfs_spiffs_conf_t conf = {
        .base_path = "/spiffs",
        .partition_label = "spiffs",
        .max_files = 10,
        .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        ESP_LOGE("SPIFFS", "Failed to mount or format filesystem : %s", esp_err_to_name(ret));
    } else {
        ESP_LOGI("SPIFFS", "SPIFFS mounted successfully");

        // 打印分区信息
        size_t total = 0, used = 0;
        ret = esp_spiffs_info(conf.partition_label, &total, &used);
        if (ret == ESP_OK) {
            ESP_LOGI("SPIFFS", "Partition size: total=%d, used=%d", total, used);
        }

        // 遍历 /spiffs 下的文件和子目录
        DIR *dir = opendir("/spiffs");
        if (dir != NULL) {
            struct dirent *ent;
            while ((ent = readdir(dir)) != NULL) {
                ESP_LOGI("SPIFFS", "File: %s", ent->d_name);
            }
            closedir(dir);
        } else {
            ESP_LOGE("SPIFFS", "Failed to open /spiffs directory");
        }
    }
}


void app_main(void) {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    printf("ESP32S3 Boot Success...\n");
    // 初始化 WiFi SoftAP
    wifi_init_softap();

    //初始化网络协议栈
    //ESP_ERROR_CHECK(esp_netif_init());
    //ESP_ERROR_CHECK(esp_event_loop_create_default());

    init_spiffs();
    // 启动 HTTP 服务

    platform_init();
    //camera_init();
    // 初始化摄像头（GPIO + SCCB + 寄存器配置）
    //ov7670_config();

    printf("系统初始化完成，等待客户端连接...\n");
}
