#ifndef WEBSERVER_BASE_H
#define WEBSERVER_BASE_H

#pragma once

#include "esp_http_server.h"

esp_err_t webserver_base_register(httpd_handle_t server);

int ota_get_result(void);

#endif  //define the head of webserver_base
