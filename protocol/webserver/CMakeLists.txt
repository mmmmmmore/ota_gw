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
        esp_jpeg jpeg 
        ov7670_handler motor_handler init ota_handler
        sccb camera jpeg common_gpio control_mgmt
        json
        )
