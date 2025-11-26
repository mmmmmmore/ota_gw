idf_component_register(
    SRCS "sccb.c"
    INCLUDE_DIRS "."
    REQUIRES 
        driver camera common_gpio jpeg  
        ov7670_handler
        webserver wifi_ap
    )
