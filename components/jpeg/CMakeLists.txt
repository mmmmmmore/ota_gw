idf_component_register(
    SRCS "jpeg.c" "tiny_jpeg.c"
    INCLUDE_DIRS "."
    REQUIRES  
        esp_new_jpeg nvs_flash driver  
        camera common_gpio sccb 
    )
