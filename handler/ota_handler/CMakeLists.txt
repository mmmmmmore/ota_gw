idf_component_register(
    SRCS 
        "ota_handler.c"
    INCLUDE_DIRS 
        "."
    REQUIRES 
        esp_wifi esp_http_server nvs_flash driver 
        esp_http_client esp_https_ota esp_system esp_app_format
        mbedtls
        common_gpio  
        webserver wifi_ap tcp_server
        cJSON
    EMBED_TXTFILES 
        "server_cert.pem"
        )
