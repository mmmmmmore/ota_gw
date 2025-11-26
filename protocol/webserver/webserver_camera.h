#ifndef WEBSERVER_CAMERA_H
#define WEBSERVER_CAMERA_H

#pragma once

#include "esp_http_server.h"

esp_err_t webserver_camera_register(httpd_handle_t server);



#endif