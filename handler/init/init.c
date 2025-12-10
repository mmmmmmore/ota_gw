// handler/init/init.c
#include "init.h"
#include "webserver.h"
#include "common_gpio.h"
#include "tcp_server.h"
#include "otaapp.h"
#include "esp_log.h"
#include "client_register.h"
#include "msg_handler.h"

void platform_init(void) {
    // 初始化 NVS 已在 app_main 中完成
    
    //initiated the GPIOs and setup default config
    common_gpio_init();               // 初始化所有 GPIO from components/common_gpio
    ledc_init();
    
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


