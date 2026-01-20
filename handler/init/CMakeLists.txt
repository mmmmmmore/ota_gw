idf_component_register(
    SRCS "init.c"
    INCLUDE_DIRS "."
    REQUIRES 
        driver common_gpio 
        client_register otaapp ledapp
        webserver wifi_ap tcp_server
        cJSON msg_handler gnss_handler
    )
