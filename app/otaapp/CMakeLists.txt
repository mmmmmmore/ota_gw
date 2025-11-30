idf_component_register(
    SRCS "otaapp.c"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi esp_http_server nvs_flash driver )
