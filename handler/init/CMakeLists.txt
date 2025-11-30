idf_component_register(
    SRCS "init.c"
    INCLUDE_DIRS "."
    REQUIRES 
        driver common_gpio 
        webserver wifi_ap 
    )
