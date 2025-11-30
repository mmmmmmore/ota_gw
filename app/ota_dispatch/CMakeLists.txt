idf_component_register(
    SRCS "ota_dispatch.c"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi esp_http_server nvs_flash driver )
