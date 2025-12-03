idf_component_register(
    SRCS "msg_handler.c"
    INCLUDE_DIRS "."
    REQUIRES esp_log cjson client_register ota_handler
)
