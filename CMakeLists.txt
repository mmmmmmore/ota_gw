cmake_minimum_required(VERSION 3.5)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# 添加额外组件目录
set(EXTRA_COMPONENT_DIRS 
        app/client_register
        app/otaapp
        app/ledapp
        components/common_gpio
        protocol/webserver
        protocol/wifi_ap 
        protocol/tcp_server
        handler/init 
        handler/ota_handler
        spiffs 
        utility/cJSON
        )

# ====== 新增版本号配置 ======
# 可以在命令行通过 -DPROJECT_VER=1.0.1 覆盖
set(PROJECT_VER "2.0.1" CACHE STRING "Project version")

# 2.0.1 as big change version, add ota feature

# 定义项目名称和版本
project(ota_gw VERSION ${PROJECT_VER})

# SPIFFS 构建规则（如果启用）
spiffs_create_partition_image(spiffs spiffs FLASH_IN_PROJECT "spiffs")

# ====== 新增固件重命名规则 ======
# 编译完成后，将生成的固件文件重命名为带版本号的文件
# ====== 固件重命名规则 ======
# 只有在主固件 bin 存在时才复制并加版本号
# ====== 固件重命名规则 ======
# 依赖于生成的 bin 文件，而不是 elf
#add_custom_command(
#    TARGET ${CMAKE_PROJECT_NAME}.elf POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_if_different
#            ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
#            ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}_v${PROJECT_VER}.bin
#    COMMENT "Renaming firmware to ${CMAKE_PROJECT_NAME}_v${PROJECT_VER}.bin"
#)


#add_custom_target(rename_firmware ALL
#    DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}_v${PROJECT_VER}.bin
#)


