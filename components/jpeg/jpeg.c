#include "jpeg.h"
#include "tiny_jpeg.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "esp_log.h"
#include "esp_heap_caps.h"

static const char *TAG = "jpeg";

typedef struct {
    uint8_t* buffer;
    size_t size;
    size_t capacity;
} jpeg_mem_context;

static void jpeg_mem_writer(void* context, void* data, int size) {
    jpeg_mem_context* ctx = (jpeg_mem_context*)context;
    if (ctx->size + size > ctx->capacity) {
        size_t new_capacity = ctx->capacity * 2;
        while (ctx->size + size > new_capacity) {
            new_capacity *= 2;
        }

        uint8_t* new_buffer = heap_caps_realloc(ctx->buffer, new_capacity, MALLOC_CAP_SPIRAM);
        if (!new_buffer) {
            ESP_LOGE(TAG, "Failed to realloc JPEG buffer");
            return;
        }

        ctx->buffer = new_buffer;
        ctx->capacity = new_capacity;
        ESP_LOGW(TAG, "JPEG buffer expanded to %d bytes", new_capacity);
    }

    memcpy(ctx->buffer + ctx->size, data, size);
    ctx->size += size;
}

uint8_t* jpeg_encode_rgb565(const uint8_t* rgb565_data, size_t rgb565_len, uint16_t width, uint16_t height, size_t* jpeg_len) {
    if (!rgb565_data || rgb565_len == 0 || width == 0 || height == 0 || !jpeg_len) {
        ESP_LOGE(TAG, "Invalid input parameters");
        return NULL;
    }

    size_t pixel_count = width * height;
    if (rgb565_len < pixel_count * 2) {
        ESP_LOGE(TAG, "RGB565 data length too small");
        return NULL;
    }

    size_t initial_jpeg_size = pixel_count;
    ESP_LOGI(TAG, "Allocating initial JPEG buffer: %d bytes", initial_jpeg_size);

    uint8_t* jpeg_buffer = heap_caps_malloc(initial_jpeg_size, MALLOC_CAP_SPIRAM);
    if (!jpeg_buffer) {
        ESP_LOGE(TAG, "Failed to allocate JPEG buffer");
        return NULL;
    }

    size_t rgb888_size = pixel_count * 3;
    ESP_LOGI(TAG, "Allocating RGB888 buffer: %d bytes", rgb888_size);

    uint8_t* rgb888 = heap_caps_malloc(rgb888_size, MALLOC_CAP_SPIRAM);
    if (!rgb888) {
        ESP_LOGE(TAG, "Failed to allocate RGB888 buffer");
        free(jpeg_buffer);
        return NULL;
    }

    ESP_LOGI(TAG, "Converting RGB565 to RGB888...");
    for (size_t i = 0; i < pixel_count; i++) {
        uint16_t pixel = ((uint16_t*)rgb565_data)[i];
        uint8_t r = (pixel >> 11) & 0x1F;
        uint8_t g = (pixel >> 5) & 0x3F;
        uint8_t b = pixel & 0x1F;

        rgb888[i * 3 + 0] = r << 3;
        rgb888[i * 3 + 1] = g << 2;
        rgb888[i * 3 + 2] = b << 3;

        if (i == 0) {
            ESP_LOGI(TAG, "First pixel RGB565: 0x%04X → RGB888: R=%d G=%d B=%d",
                     pixel, rgb888[0], rgb888[1], rgb888[2]);
        }
    }

    jpeg_mem_context ctx = {
        .buffer = jpeg_buffer,
        .size = 0,
        .capacity = initial_jpeg_size
    };

    ESP_LOGI(TAG, "Starting JPEG encoding...");
    tje_encode_with_func(jpeg_mem_writer, &ctx, 3, width, height, 3, rgb888);

    free(rgb888);

    if (ctx.size == 0) {
        ESP_LOGE(TAG, "JPEG encoding returned size 0");
        free(ctx.buffer);
        return NULL;
    }

    ESP_LOGI(TAG, "JPEG encoding successful: %d bytes", ctx.size);
    *jpeg_len = ctx.size;
    return ctx.buffer;
}
