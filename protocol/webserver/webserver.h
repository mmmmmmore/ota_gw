#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "esp_http_server.h"

// 启动 OTA Gateway WebServer
httpd_handle_t start_webserver_otagw(void);

// 停止 OTA Gateway WebServer
void stop_webserver_otagw(httpd_handle_t server);

#endif // WEBSERVER_OTAGW_H
