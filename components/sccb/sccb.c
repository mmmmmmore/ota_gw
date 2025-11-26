#include "sccb.h"
#include "camera_reg.h"
#include "common_gpio.h"

esp_err_t sccb_write(uint8_t reg_addr, uint8_t data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OV7670_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t sccb_read(uint8_t reg_addr, uint8_t *data) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OV7670_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg_addr, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) return ret;

    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (OV7670_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read_byte(cmd, data, I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t ov7670_init(void) {
    esp_err_t ret;

    // Reset camera
    ret = sccb_write(REG_COM7, COM7_RESET);
    if (ret != ESP_OK) return ret;
    vTaskDelay(pdMS_TO_TICKS(100));

    // Set RGB output format
    ret = sccb_write(REG_COM7, COM7_RGB | COM7_FMT_QVGA);
    if (ret != ESP_OK) return ret;

    // Set RGB565 format
    ret = sccb_write(REG_COM15, COM15_RGB565 | COM15_R00FF);
    if (ret != ESP_OK) return ret;

    // Set scaling and resolution for QVGA
    ret = sccb_write(REG_SCALING_DCWCTR, 0x22); // Downsample by 2
    if (ret != ESP_OK) return ret;
    ret = sccb_write(REG_SCALING_PCLK, 0xF2);   // PCLK settings
    if (ret != ESP_OK) return ret;
    ret = sccb_write(REG_SCALING_XSC, 0x3A);    // Horizontal scaling
    if (ret != ESP_OK) return ret;
    ret = sccb_write(REG_SCALING_YSC, 0x35);    // Vertical scaling
    if (ret != ESP_OK) return ret;

    // Additional recommended settings can be added here

    return ESP_OK;
}
