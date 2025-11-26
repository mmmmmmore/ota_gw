#include "webserver_camera.h"
#include "esp_log.h"
#include "esp_http_server.h"
#include "ov7670_handler.h"
#include <string.h>

static const char *TAG = "webserver_camera";
static bool stream_active = false;

static esp_err_t stream_handler(httpd_req_t *req) {
    ESP_LOGI(TAG, "HTTP GET /stream only print ....");
    return ESP_OK;
}

/*
static esp_err_t stream_handler(httpd_req_t *req) {
    ESP_LOGI(TAG, "HTTP GET /stream only print ....");

    if (!stream_active) {
        ESP_LOGW(TAG, "Stream is not active");
        httpd_resp_sendstr(req, "Stream is not active");
        return ESP_OK;
    }

    httpd_resp_set_type(req, "multipart/x-mixed-replace; boundary=frame");

    while (stream_active) {
        uint8_t *jpeg_data = NULL;
        size_t jpeg_size = 0;

        if (ov7670_handler_get_jpeg(&jpeg_data, &jpeg_size) != ESP_OK || !jpeg_data) {
            ESP_LOGE(TAG, "Failed to get JPEG frame");
            break;
        }

        char part_header[64];
        int header_len = snprintf(part_header, sizeof(part_header),
                                  "--frame\r\nContent-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n",
                                  (unsigned int)jpeg_size);

        if (httpd_resp_send_chunk(req, part_header, header_len) != ESP_OK ||
            httpd_resp_send_chunk(req, (const char *)jpeg_data, jpeg_size) != ESP_OK ||
            httpd_resp_send_chunk(req, "\r\n", 2) != ESP_OK) {
            ESP_LOGW(TAG, "Client disconnected or send failed");
            free(jpeg_data);
            break;
        }

        free(jpeg_data);
        vTaskDelay(pdMS_TO_TICKS(100)); // ~10 fps
    }

    httpd_resp_send_chunk(req, "--frame--\r\n", strlen("--frame--\r\n"));
    ESP_LOGI(TAG, "Stream ended");
    return ESP_OK;
}
*/


static esp_err_t snapshot_handler(httpd_req_t *req) {
    ESP_LOGI(TAG, "HTTP GET /snapshot");
    return ESP_OK;
}


/*
static esp_err_t snapshot_handler(httpd_req_t *req) {
    ESP_LOGI(TAG, "HTTP GET /snapshot");

    uint8_t *jpeg_data = NULL;
    size_t jpeg_size = 0;

    if (ov7670_handler_get_jpeg(&jpeg_data, &jpeg_size) != ESP_OK || !jpeg_data) {
        ESP_LOGE(TAG, "Failed to get JPEG snapshot");
        httpd_resp_send_500(req);
        return ESP_FAIL;
    }

    httpd_resp_set_type(req, "image/jpeg");
    esp_err_t res = httpd_resp_send(req, (const char *)jpeg_data, jpeg_size);
    free(jpeg_data);
    return res;
}
*/



static esp_err_t start_handler(httpd_req_t *req) {
    stream_active = true;
    ESP_LOGI(TAG, "Stream started");
    httpd_resp_send(req, "Stream started", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t stop_handler(httpd_req_t *req) {
    stream_active = false;
    ESP_LOGI(TAG, "Stream stopped");
    httpd_resp_send(req, "Stream stopped", HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t webserver_camera_register(httpd_handle_t server) {
    httpd_uri_t stream_uri = {
        .uri       = "/stream",
        .method    = HTTP_GET,
        .handler   = stream_handler,
        .user_ctx  = NULL
    };

    httpd_uri_t snapshot_uri = {
        .uri       = "/snapshot",
        .method    = HTTP_GET,
        .handler   = snapshot_handler,
        .user_ctx  = NULL
    };

    httpd_uri_t start_uri = {
        .uri = "/start",
        .method = HTTP_GET,
        .handler = start_handler,
        .user_ctx = NULL
    };

    httpd_uri_t stop_uri = {
        .uri = "/stop",
        .method = HTTP_GET,
        .handler = stop_handler,
        .user_ctx = NULL
    };

    ESP_LOGI(TAG, "Registering camera HTTP handlers");

    esp_err_t res = httpd_register_uri_handler(server, &stream_uri);
    if (res != ESP_OK) return res;

    res = httpd_register_uri_handler(server, &snapshot_uri);
    if (res != ESP_OK) return res;

    res = httpd_register_uri_handler(server, &start_uri);
    if (res != ESP_OK) return res;

    res = httpd_register_uri_handler(server, &stop_uri);
    return res;
}
