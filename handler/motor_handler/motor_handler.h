#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"

// 接收角度和距离参数，控制电机
esp_err_t motor_handler_update(int angle, int distance);

#ifdef __cplusplus
}
#endif
