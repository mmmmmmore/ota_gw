#include "spiffs.h"
#include "esp_log.h"
#include "esp_spiffs.h"




void init_spiffs(){
    esp_vfs_spiffs_conf_t conf ={
        .base_path = "/spiffs",
        .partition_label = "spiffs",
        .max_files =5,
        .format_if_mount_failed = true
    } ;
    
    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK){
        ESP_LOGE("SPIFFS:","Failed to mount or format filesystem : %s", esp_err_to_name(ret));
    }else{
        ESP_LOGI("SPIFFS:","SPIFFS mounted successfully");
    }
}