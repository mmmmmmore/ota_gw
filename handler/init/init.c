// handler/init/init.c
#include "init.h"
#include "webserver.h"
#include "sccb.h"
#include "jpeg.h"
#include "control_mgmt.h"
//#include "joystick_handler.h"
//#include "webserver_control.h"
#include "common_gpio.h"
#include "camera.h"
//#include "log_handler.h"

void platform_init(void) {
    // 初始化 NVS 已在 app_main 中完成
    
    //initiated the GPIOs and setup default config
    common_gpio_init();               // 初始化所有 GPIO from components/common_gpio
    ledc_init();
    i2c_master_init();                  
    // above from components/common_gpio

    control_manager_init(); // init the control memory   
    //init_log_handler();       // 初始化日志模块
    //camera_init();            // 初始化摄像头
    
    //sccb_init();
    //stream_handler_init();    // 初始化图像流处理
    //joystick_handler_init();  // 初始化摇杆处理
    //webserver_control_init(); // 初始化 WebServer 控制逻辑
}

