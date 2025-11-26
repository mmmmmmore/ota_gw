// spi.c
#include "spi.h"
#include "common_gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"

static const char *TAG = "SPI";
static spi_device_handle_t spi_clients[SPI_CLIENT_MAX];

esp_err_t spi_master_init(void) {
    spi_bus_config_t buscfg = {
        .mosi_io_num = GPIO_SPI_MOSI,
        .miso_io_num = GPIO_SPI_MISO,
        .sclk_io_num = GPIO_SPI_SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096
    };

    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init SPI bus");
        return ret;
    }

    // 为每个 Client 添加设备
    int cs_pins[SPI_CLIENT_MAX] = {GPIO_SPI_CS1, GPIO_SPI_CS2, GPIO_SPI_CS3};
    for (int i = 0; i < SPI_CLIENT_MAX; i++) {
        spi_device_interface_config_t devcfg = {
            .clock_speed_hz = 1*1000*1000,   // 1 MHz
            .mode = 0,                       // SPI mode 0
            .spics_io_num = cs_pins[i],
            .queue_size = 3,
        };
        ret = spi_bus_add_device(SPI2_HOST, &devcfg, &spi_clients[i]);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to add SPI client %d", i);
            return ret;
        }
    }
    ESP_LOGI(TAG, "SPI Master initialized");
    return ESP_OK;
}

esp_err_t spi_master_send(spi_client_id_t client, const uint8_t *data, size_t len) {
    spi_transaction_t t = {
        .length = len * 8,
        .tx_buffer = data,
    };
    return spi_device_transmit(spi_clients[client], &t);
}

esp_err_t spi_master_receive(spi_client_id_t client, uint8_t *buffer, size_t len) {
    spi_transaction_t t = {
        .length = len * 8,
        .rx_buffer = buffer,
    };
    return spi_device_transmit(spi_clients[client], &t);
}
