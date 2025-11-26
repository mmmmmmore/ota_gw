idf_component_register(
    SRCS 
        "spiffs.c"
    INCLUDE_DIRS 
        "."
    REQUIRES
        spiffs
)


#spiffs_create_partition_image(spiffs ${CMAKE_CURRENT_DIRS}/static FLASH_IN_PROJECT)
