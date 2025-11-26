#include "camera.h"
#include "common_gpio.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <stdlib.h>
#include "esp_rom_sys.h"
#include "esp_heap_caps.h"

static const char *TAG = "camera";

static uint8_t *frame_buffer = NULL;
static bool camera_running = false;

esp_err_t camera_init(void) {
    ESP_LOGI(TAG, "Initializing camera module...");

    frame_buffer = (uint8_t *)heap_caps_malloc(CAMERA_FRAME_SIZE, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (!frame_buffer) {
        ESP_LOGE(TAG, "Failed to allocate frame buffer from PSRAM");
        return ESP_ERR_NO_MEM;
    }

    ESP_LOGI(TAG, "Frame buffer allocated at: %p", frame_buffer);
    camera_running = true;
    return ESP_OK;
}

void camera_stop(void) {
    camera_running = false;
    if (frame_buffer) {
        free(frame_buffer);
        frame_buffer = NULL;
    }
}


uint8_t* camera_capture_frame(void) {
    if (!camera_running || !frame_buffer) {
        ESP_LOGW(TAG, "Camera not running or buffer not allocated");
        return NULL;
    }

    /*
    // 显式定义 GPIO 数据线数组，避免 GPIO_D0 + bit 越界
    gpio_num_t data_pins[8] = {
        GPIO_D0, GPIO_D1, GPIO_D2, GPIO_D3,
        GPIO_D4, GPIO_D5, GPIO_D6, GPIO_D7
    };

    // 检查 GPIO 编号合法性
    for (int i = 0; i < 8; i++) {
        if (data_pins[i] < GPIO_NUM_0 || data_pins[i] >= GPIO_NUM_MAX) {
            ESP_LOGE(TAG, "Invalid data GPIO: %d", data_pins[i]);
            return NULL;
        }
    }
    
    // 重置 FIFO 读指针
    gpio_set_level(GPIO_RRST, 0);
    gpio_set_level(GPIO_RCLK, 0);
    esp_rom_delay_us(1);
    gpio_set_level(GPIO_RCLK, 1);
    esp_rom_delay_us(1);
    gpio_set_level(GPIO_RRST, 1);

    // 启用 FIFO 输出
    gpio_set_level(GPIO_OE, 0);

    // 读取 RGB565 像素（每像素 2 字节）
    for (int i = 0; i < CAMERA_FRAME_WIDTH * CAMERA_FRAME_HEIGHT; i++) {
        uint16_t pixel = 0;

        for (int byte_index = 0; byte_index < 2; byte_index++) {
            gpio_set_level(GPIO_RCLK, 1);
            esp_rom_delay_us(1);
            gpio_set_level(GPIO_RCLK, 0);
            esp_rom_delay_us(1);

            uint8_t byte = 0;
            for (int bit = 0; bit < 8; bit++) {
                int level = gpio_get_level(data_pins[bit]);
                byte |= (level << bit);
            }

            if (byte_index == 0) {
                pixel = byte << 8;  // 高字节
            } else {
                pixel |= byte;      // 低字节
            }
        }

        // 写入帧缓冲区
        frame_buffer[i * 2]     = (pixel >> 8) & 0xFF;
        frame_buffer[i * 2 + 1] = pixel & 0xFF;
    }

    // 禁用 FIFO 输出
    gpio_set_level(GPIO_OE, 1);
    */
    ESP_LOGI(TAG, "RGB565 frame captured.");
    return frame_buffer;
}
