#ifndef WEBSERVER_H
#define WEBSERVER_H

#pragma once
#include "esp_http_server.h"

// 启动 HTTP WebServer，注册 /toggle, /image, /stream 路由
httpd_handle_t start_webserver(void);
esp_err_t webserver_init(httpd_handle_t server);

#endif