#ifndef WEBSERVER_OTAGW_H
#define WEBSERVER_OTAGW_H

#include "esp_http_server.h"

// 启动 WebServer
httpd_handle_t start_webserver(void);

// 停止 WebServer
void stop_webserver(httpd_handle_t server);

#endif // WEBSERVER_OTAGW_H
