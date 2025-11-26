idf_component_register(
    SRCS 
        "common_gpio.c"
    INCLUDE_DIRS 
        "."
    REQUIRES 
        esp_wifi esp_http_server nvs_flash driver heap esp_common 
        camera jpeg sccb
        )
