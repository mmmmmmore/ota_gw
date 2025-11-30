idf_component_register(
    SRCS "tcp_server.c"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi esp_http_server nvs_flash driver )
