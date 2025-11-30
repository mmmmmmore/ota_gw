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




void check_psram_status() {
    ESP_LOGI("PSRAM", "PSRAM size: %d bytes", esp_psram_get_size());
    ESP_LOGI("PSRAM", "Free heap: %d bytes", esp_get_free_heap_size());

    if (esp_psram_is_initialized()) {
        ESP_LOGI("PSRAM", "PSRAM is initialized and ready.");
    } else {
        ESP_LOGE("PSRAM", "PSRAM is NOT initialized.");
    }

    void *test_ptr = heap_caps_malloc(1024, MALLOC_CAP_SPIRAM);
    if (test_ptr) {
        ESP_LOGI("PSRAM", "Successfully allocated 1KB from PSRAM.");
        free(test_ptr);
    } else {
        ESP_LOGE("PSRAM", "Failed to allocate memory from PSRAM.");
    }
}


void init_spiffs(){
    esp_vfs_spiffs_conf_t conf ={
        .base_path = "/spiffs",
        .partition_label = "spiffs",
        .max_files =5,
        .format_if_mount_failed = true
    } ;
    
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK){
        ESP_LOGE("SPIFFS:","Failed to mount or format filesystem : %s", esp_err_to_name(ret));
    }else{
        ESP_LOGI("SPIFFS:","SPIFFS mounted successfully");
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

    check_psram_status();
    init_spiffs();
    // 启动 HTTP 服务

    platform_init();
    //camera_init();
    // 初始化摄像头（GPIO + SCCB + 寄存器配置）
    //ov7670_config();

    printf("系统初始化完成，等待客户端连接...\n");
}
