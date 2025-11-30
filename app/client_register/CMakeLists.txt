idf_component_register(
    SRCS "client_register.c"
    INCLUDE_DIRS "."
    REQUIRES esp_wifi esp_http_server nvs_flash driver 
    cJSON
    )
