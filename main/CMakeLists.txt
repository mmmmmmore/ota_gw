idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES 
        esp_wifi esp_http_server nvs_flash driver 
        client_register otaapp ledapp
        common_gpio init
        webserver wifi_ap tcp_server
        spiffs 
        esp_system esp_psram
)
