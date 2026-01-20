idf_component_register(
    SRCS 
       "webserver.c"
    INCLUDE_DIRS 
        "."
    REQUIRES 
        esp_wifi esp_http_server nvs_flash driver  
        init 
        common_gpio
        otaapp
        cJSON
        gnss_handler
        )
