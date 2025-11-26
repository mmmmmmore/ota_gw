#include "ov7670_handler.h"
#include "camera.h"
#include "jpeg.h"
#include "esp_log.h"
#include "common_gpio.h"
#include <stdlib.h>

static const char *TAG = "ov7670_handler";

esp_err_t ov7670_handler_get_jpeg(uint8_t **jpeg_data, size_t *jpeg_size) {
    if (!jpeg_data || !jpeg_size) {
        ESP_LOGE(TAG, "Invalid output parameters");
        return ESP_ERR_INVALID_ARG;
    }

    ESP_LOGI(TAG, "Capturing frame from camera...");
    uint8_t *raw_data = camera_capture_frame();
    if (!raw_data) {
        ESP_LOGE(TAG, "Failed to capture frame");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "Encoding frame to JPEG...");
    *jpeg_data = jpeg_encode_rgb565(raw_data, CAMERA_FRAME_SIZE, CAMERA_FRAME_WIDTH, CAMERA_FRAME_HEIGHT, jpeg_size);
    if (!*jpeg_data || *jpeg_size == 0) {
        ESP_LOGE(TAG, "JPEG encoding failed");
        return ESP_FAIL;
    }

    ESP_LOGI(TAG, "JPEG encoding successful, size: %d bytes", (int)*jpeg_size);
    return ESP_OK;
}
