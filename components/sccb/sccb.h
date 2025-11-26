#ifndef SCCB_H
#define SCCB_H

#pragma once

#include "driver/i2c.h"
#include "esp_err.h"

// OV7670 默认地址（7位地址为 0x21，8位地址为 0x42）
#define OV7670_ADDR         0x21

esp_err_t sccb_write(uint8_t reg_addr, uint8_t data);
esp_err_t sccb_read(uint8_t reg_addr, uint8_t *data);
esp_err_t ov7670_init(void);


#endif
