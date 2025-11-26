#include "webserver.h"
#include "webserver_base.h"
#include "webserver_camera.h"
#include "webserver_motor.h"
#include "esp_http_server.h"
#include "ov7670_handler.h"
#include "jpeg.h"
#include "esp_log.h"
#include "esp_heap_caps.h"
#include <string.h>

#define STREAM_FRAME_INTERVAL_MS 100 // stream image flash time interval ms

static const char *TAG = "webserver";




/*  below if new webserver initiate*/
httpd_handle_t start_webserver(void) {
    ESP_LOGI(TAG, "Starting webserver...");
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 16; //setup more uri for link
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        ESP_LOGI(TAG, "Webserver started");
        webserver_init(server);

    } else {
        ESP_LOGE(TAG, "Failed to start webserver");
    }

    return server;
}


esp_err_t webserver_init(httpd_handle_t server) {
    ESP_LOGI(TAG, "Registering base URI handlers...");
    ESP_ERROR_CHECK(webserver_base_register(server));

    ESP_LOGI(TAG, "Registering camera URI handlers...");
    ESP_ERROR_CHECK(webserver_camera_register(server));

    ESP_LOGI(TAG, "Registering motor URI handlers...");
    ESP_ERROR_CHECK(webserver_motor_register(server));

    ESP_LOGI(TAG, "All URI handlers registered successfully.");
    return ESP_OK;
}
