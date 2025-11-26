// spi.h
#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include "driver/spi_master.h"

// SPI Client ID 枚举（对应不同片选）
typedef enum {
    SPI_CLIENT_1 = 0,
    SPI_CLIENT_2,
    SPI_CLIENT_3,
    SPI_CLIENT_MAX
} spi_client_id_t;

// 初始化 SPI Master
esp_err_t spi_master_init(void);

// 向指定 Client 发送数据
esp_err_t spi_master_send(spi_client_id_t client, const uint8_t *data, size_t len);

// 从指定 Client 接收数据
esp_err_t spi_master_receive(spi_client_id_t client, uint8_t *buffer, size_t len);

#endif // SPI_H
