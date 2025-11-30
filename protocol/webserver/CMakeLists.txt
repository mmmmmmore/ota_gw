idf_component_register(
    SRCS 
        "webserver.c"
        "webserver_base.c"
        "webserver_camera.c"
        "webserver_motor.c"
    INCLUDE_DIRS 
        "."
    REQUIRES 
        esp_wifi esp_http_server nvs_flash driver  
        init ota_handler
         common_gpio json
        )
