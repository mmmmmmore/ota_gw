// handler/init/init.c
#include "init.h"
#include "webserver.h"
#include "common_gpio.h"
#include "tcp_server.h"
#include "otaapp.h"
#include "esp_log.h"
#include "client_register.h"
#include "msg_handler.h"
#include "gnss_handler.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"



void platform_init(void) {
    // 初始化 NVS 已在 app_main 中完成
    
    //initiated the GPIOs and setup default config
    common_gpio_init();               // 初始化所有 GPIO from components/common_gpio
    
    //initialize GNSS handler for time calibration
    gnss_handler_init();              // 初始化 GNSS 处理器
    gnss_handler_start_task();        // 启动 GNSS 数据接收任务
    
    //start the tcp server
    gw_tcp_servers_init();

    //    msg_handler_init();
    //start the client register module
    client_register_init();
    //start ota app
    //ota_dispatch_init();

    //start the webserver
    start_webserver_otagw();

}


void system_monitor_task(void *pvParameters){
    while (1)
    {
        //ram status
        size_t free_heap = esp_get_free_heap_size();
        size_t min_heap = esp_get_minimum_free_heap_size();
        ESP_LOGI("SYS_INFO", "Free heap : %d, Min Heap : %d", free_heap, min_heap);

        //CPU status
        char stats_buff[512];
        vTaskGetRunTimeStats(stats_buff);
        ESP_LOGI("SYS_INFO", "Task CPU usage : \n%s", stats_buff);

        vTaskDelay(pdMS_TO_TICKS(10000));
    }
    
}