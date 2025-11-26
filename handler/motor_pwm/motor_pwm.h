#ifndef MOTOR_PWM_H
#define MOTOR_PWM_H


#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"

// 电机编号范围：1 ~ 4
#define MOTOR_COUNT 4

// 每个电机的方向和 PWM 引脚定义
typedef struct {
    gpio_num_t in1_gpio;
    gpio_num_t in2_gpio;
    gpio_num_t pwm_gpio;
    ledc_channel_t pwm_channel;
} motor_gpio_config_t;

// 初始化所有电机的 GPIO 和 PWM
esp_err_t pwm_init_all_motors(void);

// 设置指定电机的方向（true = 正转，false = 反转）
void pwm_set_motor_direction(int motor_id, bool forward);

// 设置指定电机的速度（0 ~ 100）
void pwm_set_motor_speed(int motor_id, int speed_percent);

// 停止所有电机
void pwm_stop_all_motors(void);



#endif

