#pragma once

#include <stdint.h>
#include <stddef.h>
#include "esp_err.h"

// Interface to capture and encode a JPEG frame from OV7670
esp_err_t ov7670_handler_get_jpeg(uint8_t **jpeg_data, size_t *jpeg_size);
