#ifndef COMMON_GPIO_H
#define COMMON_GPIO_H

#pragma once

#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_err.h"

// ======================= Ublox NEO-6M GNSS 模块 =======================
#define GPIO_UBLOX_TXD       4      // Ublox-NEO-6M TXD pin (UART TX)
#define GPIO_UBLOX_RXD       5      // Ublox-NEO-6M RXD pin (UART RX)
#define GPIO_UBLOX_PPS       6      // Ublox-NEO-6M PPS pin (Pulse Per Second)

// ======================= I2C 引脚定义 =======================
#define GPIO_SCL             19     // I2C Clock
#define GPIO_SDA             20     // I2C Data


// ======================= UART 参数定义 =======================
#define UART_UBLOX_PORT      UART_NUM_1       // 使用 UART1 连接 Ublox
#define UART_UBLOX_BAUDRATE  9600             // Ublox NEO-6M 默认波特率
#define UART_UBLOX_BUFFER_SIZE  256           // UART 缓冲区大小

// ======================= I2C 参数定义 =======================
#define I2C_MASTER_NUM       I2C_NUM_0          // I2C 控制器编号
#define I2C_MASTER_FREQ_HZ   100000             // I2C 通信频率（100kHz）
#define I2C_MASTER_TX_BUF_DISABLE 0             // 不使用 TX 缓冲区
#define I2C_MASTER_RX_BUF_DISABLE 0             // 不使用 RX 缓冲区
#define I2C_MASTER_TIMEOUT_MS    1000           // I2C 操作超时时间（毫秒）

void common_gpio_init(void);
void i2c_master_init(void);
void uart_ublox_init(void);

#endif
