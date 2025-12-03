idf_component_register(
    SRCS "msg_handler.c"
    INCLUDE_DIRS "."
    REQUIRES driver  client_register ota_handler 
    cJSON
)
