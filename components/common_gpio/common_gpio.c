#include "common_gpio.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "driver/uart.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdio.h>
#include <stdlib.h>

static const char *TAG = "common_gpio";

// ======================= GPIO 总入口函数 =======================
void common_gpio_init(void) {
    // Initialize Ublox UART once here so downstream modules don't reinstall
    uart_ublox_init();

    ESP_LOGI(TAG, "GPIO initialization completed");
}


// ======================= I2C 初始化 =======================
void i2c_master_init(void) {
    ESP_LOGI(TAG, "Initializing I2C master...");

    // 正确配置 GPIO 模式
    gpio_config_t i2c_gpio_conf = {
        .mode = GPIO_MODE_INPUT_OUTPUT_OD,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = (1ULL << GPIO_SCL) | (1ULL << GPIO_SDA)
    };
    ESP_ERROR_CHECK(gpio_config(&i2c_gpio_conf));

    // 配置 I2C 控制器
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_SDA,
        .scl_io_num = GPIO_SCL,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ
    };

    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(I2C_MASTER_NUM, conf.mode,
                                       I2C_MASTER_RX_BUF_DISABLE,
                                       I2C_MASTER_TX_BUF_DISABLE, 0));

    ESP_LOGI(TAG, "I2C master initialized");
}


// ======================= UART Ublox 初始化 =======================
void uart_ublox_init(void) {
    ESP_LOGI(TAG, "Initializing UART for Ublox NEO-6M...");
    
    // 配置 UART 参数
    uart_config_t uart_config = {
        .baud_rate = UART_UBLOX_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    
    // 安装 UART 驱动
    ESP_ERROR_CHECK(uart_driver_install(UART_UBLOX_PORT, UART_UBLOX_BUFFER_SIZE * 2, 
                                        UART_UBLOX_BUFFER_SIZE * 2, 0, NULL, 0));
    
    // 配置 UART 参数
    ESP_ERROR_CHECK(uart_param_config(UART_UBLOX_PORT, &uart_config));
    
    // 设置 UART 引脚
    ESP_ERROR_CHECK(uart_set_pin(UART_UBLOX_PORT, GPIO_UBLOX_TXD, GPIO_UBLOX_RXD, 
                                 UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    
    // 配置 PPS 引脚为输入
    gpio_config_t pps_gpio_conf = {
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = (1ULL << GPIO_UBLOX_PPS)
    };
    ESP_ERROR_CHECK(gpio_config(&pps_gpio_conf));
    
    ESP_LOGI(TAG, "UART Ublox initialized: TXD=%d, RXD=%d, PPS=%d, Baudrate=%d", 
             GPIO_UBLOX_TXD, GPIO_UBLOX_RXD, GPIO_UBLOX_PPS, UART_UBLOX_BAUDRATE);
}