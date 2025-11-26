#ifndef COMMON_GPIO_H
#define COMMON_GPIO_H

#pragma once

#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"

#pragma once

// ======================= AL422B FIFO 摄像头模块 =======================
#define GPIO_SCL             4     // SCCB 时钟线
#define GPIO_SDA             5     // SCCB 数据线

#define GPIO_D0              6     // 图像数据位 0
#define GPIO_D1              7     // 图像数据位 1
#define GPIO_D2              8     // 图像数据位 2
#define GPIO_D3              9     // 图像数据位 3
#define GPIO_D4              10    // 图像数据位 4
#define GPIO_D5              11    // 图像数据位 5
#define GPIO_D6              12    // 图像数据位 6
#define GPIO_D7              13    // 图像数据位 7

#define GPIO_VSYNC           21    // 同步信号
#define GPIO_RCLK            14    // 写入控制
#define GPIO_OE              15    // 输出使能
#define GPIO_WRST            16    // FIFO 复位
#define GPIO_RRST            17    // 读复位
#define GPIO_WEN             18    // 写使能

// ======================= TB6612FNG 电机驱动模块 =======================
// TB6612FNG #1
#define GPIO_MOTOR1_AIN1     19
#define GPIO_MOTOR1_AIN2     20
#define GPIO_MOTOR1_BIN1     38
#define GPIO_MOTOR1_BIN2     39
#define GPIO_MOTOR1_PWMA     40
#define GPIO_MOTOR1_PWMB     41

// TB6612FNG #2
#define GPIO_MOTOR2_AIN1     33
#define GPIO_MOTOR2_AIN2     34
#define GPIO_MOTOR2_BIN1     35
#define GPIO_MOTOR2_BIN2     36
#define GPIO_MOTOR2_PWMA     37
#define GPIO_MOTOR2_PWMB     1

// 共用 STBY 引脚（用于两个电机驱动器）
#define GPIO_MOTOR_STBY      42

// ======================= 其他功能引脚 =======================
#define GPIO_LED_STATUS      3     // 状态指示灯
#define GPIO_WEBSERVER_CTRL  0     // WebServer 控制信号（注意 GPIO0 启动模式影响）

// ======================= I2C 参数定义 =======================
#define I2C_MASTER_NUM       I2C_NUM_0          // 使用 I2C 控制器编号
#define I2C_MASTER_FREQ_HZ   100000             // I2C 通信频率（100kHz）
#define I2C_MASTER_TX_BUF_DISABLE 0             // 不使用 TX 缓冲区
#define I2C_MASTER_RX_BUF_DISABLE 0             // 不使用 RX 缓冲区
#define I2C_MASTER_TIMEOUT_MS    1000           // I2C 操作超时时间（毫秒）

// ======================= LEDC 参数定义 =======================
#define LEDC_TIMER           LEDC_TIMER_0       // 使用 LEDC 定时器 0
#define LEDC_MODE            LEDC_HIGH_SPEED_MODE
#define LEDC_DUTY_RES        LEDC_TIMER_10_BIT  // PWM 分辨率：10 位
#define LEDC_FREQUENCY       5000               // PWM 频率：5kHz

// 可选：电机 PWM 通道定义（根据需要使用）
#define LEDC_CHANNEL_MOTOR1_A  LEDC_CHANNEL_0
#define LEDC_CHANNEL_MOTOR1_B  LEDC_CHANNEL_1
#define LEDC_CHANNEL_MOTOR2_A  LEDC_CHANNEL_2
#define LEDC_CHANNEL_MOTOR2_B  LEDC_CHANNEL_3



void common_gpio_init(void);
void ledc_init(void);
void i2c_master_init(void);

// GPIO 初始化函数
void fifo_gpio_init(void);

// 数据读取函数（可选）
//uint8_t fifo_read_byte(void);

// 控制引脚设置函数（可选）
void fifo_set_rclk(bool level);
void fifo_set_rrst(bool level);
void fifo_set_oe(bool level);


typedef struct {
    gpio_num_t dir_gpio;         // 控制方向的 GPIO（连接 TB6621 的 IN2）
    gpio_num_t pwm_gpio;         // 控制速度的 GPIO（连接 TB6621 的 IN1）
    ledc_channel_t pwm_channel;  // PWM 通道
} motor_gpio_t;

extern const motor_gpio_t motor_fl;
extern const motor_gpio_t motor_fr;
extern const motor_gpio_t motor_rl;
extern const motor_gpio_t motor_rr;


#endif
