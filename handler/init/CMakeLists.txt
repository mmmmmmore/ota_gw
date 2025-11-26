idf_component_register(
    SRCS "init.c"
    INCLUDE_DIRS "."
    REQUIRES 
        driver camera common_gpio jpeg  sccb
        ov7670_handler motor_handler
        webserver wifi_ap control_mgmt
    )
