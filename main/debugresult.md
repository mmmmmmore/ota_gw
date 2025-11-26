2025-09-30

debug log:
lude_syscalls_impl  -u esp_vfs_include_nullfs_register && :
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: esp-idf/motor_handler/libmotor_handler.a(motor_handler.c.obj):(.literal.motor_handler_update+0x1c): undefined reference to `pwm_stop_all_motors'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: esp-idf/motor_handler/libmotor_handler.a(motor_handler.c.obj):(.literal.motor_handler_update+0x20): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: esp-idf/motor_handler/libmotor_handler.a(motor_handler.c.obj):(.literal.motor_handler_update+0x24): undefined reference to `pwm_set_motor_speed'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: esp-idf/motor_handler/libmotor_handler.a(motor_handler.c.obj): in function `motor_handler_update':
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:40:(.text.motor_handler_update+0x6): undefined reference to `pwm_stop_all_motors'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:46:(.text.motor_handler_update+0x32): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:47:(.text.motor_handler_update+0x3c): undefined reference to `pwm_set_motor_speed'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:54:(.text.motor_handler_update+0x70): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:55:(.text.motor_handler_update+0x7a): undefined reference to `pwm_set_motor_speed'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:61:(.text.motor_handler_update+0xaa): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:62:(.text.motor_handler_update+0xb4): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:63:(.text.motor_handler_update+0xbe): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:64:(.text.motor_handler_update+0xc8): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:66:(.text.motor_handler_update+0xd8): undefined reference to `pwm_set_motor_speed'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:72:(.text.motor_handler_update+0x108): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:73:(.text.motor_handler_update+0x112): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:74:(.text.motor_handler_update+0x11c): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:75:(.text.motor_handler_update+0x126): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:77:(.text.motor_handler_update+0x136): undefined reference to `pwm_set_motor_speed'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:84:(.text.motor_handler_update+0x162): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:85:(.text.motor_handler_update+0x16c): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:86:(.text.motor_handler_update+0x176): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:87:(.text.motor_handler_update+0x180): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:89:(.text.motor_handler_update+0x190): undefined reference to `pwm_set_motor_speed'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:95:(.text.motor_handler_update+0x1c0): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:96:(.text.motor_handler_update+0x1ca): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:97:(.text.motor_handler_update+0x1d4): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:98:(.text.motor_handler_update+0x1de): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:100:(.text.motor_handler_update+0x1ee): undefined reference to `pwm_set_motor_speed'
collect2: error: ld returned 1 exit status
ninja: build stopped: subcommand failed.
ninja failed with exit code 1, output of the command is in the /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/build/log/idf_py_stderr_output_33498 and /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/build/log/idf_py_stdout_output_33498
MMMBP:esp32_wifi_control maochun$ 


uccessfully created esp32s3 image.
Generated /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/build/bootloader/bootloader.bin
[3/3] cd /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_c...ct_CAM/branch/esp32_wifi_control/build/bootloader/bootloader.bin
Bootloader binary size 0x52e0 bytes. 0x2d20 bytes (35%) free.
[1091/1094] Linking CXX executable esp32_wifi_control.elf
FAILED: [code=1] esp32_wifi_control.elf 
: && /Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/xtensa-esp32s3-elf-g++ -mlongcalls -fno-builtin-memcpy -fno-builtin-memset -fno-builtin-bzero -nostartfiles  -Wl,--cref -Wl,--defsym=IDF_TARGET_ESP32S3=0 -Wl,--Map=/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/build/esp32_wifi_control.map -Wl,--no-warn-rwx-segments -Wl,--orphan-handling=error -fno-rtti -fno-lto -Wl,--gc-sections -Wl,--warn-common -T esp32s3.peripherals.ld -T esp32s3.rom.ld -T esp32s3.rom.api.ld -T esp32s3.rom.bt_funcs.ld -T esp32s3.rom.libgcc.ld -T esp32s3.rom.wdt.ld -T esp32s3.rom.version.ld -T esp32s3.rom.ble_master.ld -T esp32s3.rom.ble_50.ld -T esp32s3.rom.ble_smp.ld -T esp32s3.rom.ble_dtm.ld -T esp32s3.rom.ble_test.ld -T esp32s3.rom.ble_scan.ld -T esp32s3.rom.libc.ld -T esp32s3.rom.newlib.ld -T memory.ld -T sections.ld -u include_esp_phy_override CMakeFiles/esp32_wifi_control.elf.dir/project_elf_src_esp32s3.c.obj -o esp32_wifi_control.elf -L/Users/maochun/esp32prj/esp-idf/components/soc/esp32s3/ld   -L/Users/maochun/esp32prj/esp-idf/components/esp_rom/esp32s3/ld   -L/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/build/esp-idf/esp_system/ld   -L/Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3 esp-idf/xtensa/libxtensa.a  esp-idf/esp_app_format/libesp_app_format.a  esp-idf/esp_bootloader_format/libesp_bootloader_format.a  esp-idf/app_update/libapp_update.a  esp-idf/esp_partition/libesp_partition.a  esp-idf/efuse/libefuse.a  esp-idf/esp_security/libesp_security.a  esp-idf/esp_driver_gpio/libesp_driver_gpio.a  esp-idf/esp_pm/libesp_pm.a  esp-idf/mbedtls/libmbedtls.a  esp-idf/bootloader_support/libbootloader_support.a  esp-idf/esp_mm/libesp_mm.a  esp-idf/spi_flash/libspi_flash.a  esp-idf/esp_system/libesp_system.a  esp-idf/esp_common/libesp_common.a  esp-idf/esp_rom/libesp_rom.a  esp-idf/hal/libhal.a  esp-idf/log/liblog.a  esp-idf/heap/libheap.a  esp-idf/soc/libsoc.a  esp-idf/esp_hw_support/libesp_hw_support.a  esp-idf/freertos/libfreertos.a  esp-idf/newlib/libnewlib.a  esp-idf/pthread/libpthread.a  esp-idf/cxx/libcxx.a  esp-idf/esp_timer/libesp_timer.a  esp-idf/esp_driver_gptimer/libesp_driver_gptimer.a  esp-idf/esp_ringbuf/libesp_ringbuf.a  esp-idf/esp_driver_uart/libesp_driver_uart.a  esp-idf/app_trace/libapp_trace.a  esp-idf/esp_event/libesp_event.a  esp-idf/nvs_flash/libnvs_flash.a  esp-idf/esp_phy/libesp_phy.a  esp-idf/esp_driver_usb_serial_jtag/libesp_driver_usb_serial_jtag.a  esp-idf/esp_vfs_console/libesp_vfs_console.a  esp-idf/vfs/libvfs.a  esp-idf/lwip/liblwip.a  esp-idf/esp_netif/libesp_netif.a  esp-idf/wpa_supplicant/libwpa_supplicant.a  esp-idf/esp_coex/libesp_coex.a  esp-idf/esp_wifi/libesp_wifi.a  esp-idf/esp_driver_spi/libesp_driver_spi.a  esp-idf/esp_gdbstub/libesp_gdbstub.a  esp-idf/unity/libunity.a  esp-idf/cmock/libcmock.a  esp-idf/console/libconsole.a  esp-idf/esp_driver_pcnt/libesp_driver_pcnt.a  esp-idf/esp_driver_mcpwm/libesp_driver_mcpwm.a  esp-idf/esp_driver_i2s/libesp_driver_i2s.a  esp-idf/sdmmc/libsdmmc.a  esp-idf/esp_driver_sd_intf/libesp_driver_sd_intf.a  esp-idf/esp_driver_sdmmc/libesp_driver_sdmmc.a  esp-idf/esp_driver_sdspi/libesp_driver_sdspi.a  esp-idf/esp_driver_rmt/libesp_driver_rmt.a  esp-idf/esp_driver_tsens/libesp_driver_tsens.a  esp-idf/esp_driver_sdm/libesp_driver_sdm.a  esp-idf/esp_driver_i2c/libesp_driver_i2c.a  esp-idf/esp_driver_ledc/libesp_driver_ledc.a  esp-idf/esp_driver_twai/libesp_driver_twai.a  esp-idf/driver/libdriver.a  esp-idf/http_parser/libhttp_parser.a  esp-idf/esp-tls/libesp-tls.a  esp-idf/esp_adc/libesp_adc.a  esp-idf/esp_driver_cam/libesp_driver_cam.a  esp-idf/esp_psram/libesp_psram.a  esp-idf/esp_driver_touch_sens/libesp_driver_touch_sens.a  esp-idf/esp_eth/libesp_eth.a  esp-idf/esp_hid/libesp_hid.a  esp-idf/tcp_transport/libtcp_transport.a  esp-idf/esp_http_client/libesp_http_client.a  esp-idf/esp_http_server/libesp_http_server.a  esp-idf/esp_https_ota/libesp_https_ota.a  esp-idf/esp_https_server/libesp_https_server.a  esp-idf/esp_lcd/libesp_lcd.a  esp-idf/protobuf-c/libprotobuf-c.a  esp-idf/protocomm/libprotocomm.a  esp-idf/esp_local_ctrl/libesp_local_ctrl.a  esp-idf/espcoredump/libespcoredump.a  esp-idf/wear_levelling/libwear_levelling.a  esp-idf/fatfs/libfatfs.a  esp-idf/json/libjson.a  esp-idf/mqtt/libmqtt.a  esp-idf/nvs_sec_provider/libnvs_sec_provider.a  esp-idf/perfmon/libperfmon.a  esp-idf/rt/librt.a  esp-idf/spiffs/libspiffs.a  esp-idf/touch_element/libtouch_element.a  esp-idf/usb/libusb.a  esp-idf/wifi_provisioning/libwifi_provisioning.a  esp-idf/espressif__esp_jpeg/libespressif__esp_jpeg.a  esp-idf/wifi_ap/libwifi_ap.a  esp-idf/motor_pwm/libmotor_pwm.a  esp-idf/capture_control/libcapture_control.a  esp-idf/webserver_control/libwebserver_control.a  esp-idf/motor_handler/libmotor_handler.a  esp-idf/webserver/libwebserver.a  esp-idf/ov7670_handler/libov7670_handler.a  esp-idf/sccb/libsccb.a  esp-idf/jpeg/libjpeg.a  esp-idf/common_gpio/libcommon_gpio.a  esp-idf/camera/libcamera.a  esp-idf/main/libmain.a  esp-idf/tmp/libtmp.a  esp-idf/unity/libunity.a  esp-idf/esp_https_server/libesp_https_server.a  -u esp_system_include_coredump_init  esp-idf/wear_levelling/libwear_levelling.a  -u nvs_sec_provider_include_impl  esp-idf/protocomm/libprotocomm.a  esp-idf/console/libconsole.a  esp-idf/protobuf-c/libprotobuf-c.a  esp-idf/json/libjson.a  esp-idf/motor_pwm/libmotor_pwm.a  esp-idf/capture_control/libcapture_control.a  esp-idf/webserver_control/libwebserver_control.a  esp-idf/motor_handler/libmotor_handler.a  esp-idf/webserver/libwebserver.a  esp-idf/ov7670_handler/libov7670_handler.a  esp-idf/sccb/libsccb.a  esp-idf/jpeg/libjpeg.a  esp-idf/common_gpio/libcommon_gpio.a  esp-idf/camera/libcamera.a  esp-idf/motor_pwm/libmotor_pwm.a  esp-idf/capture_control/libcapture_control.a  esp-idf/webserver_control/libwebserver_control.a  esp-idf/motor_handler/libmotor_handler.a  esp-idf/webserver/libwebserver.a  esp-idf/ov7670_handler/libov7670_handler.a  esp-idf/sccb/libsccb.a  esp-idf/jpeg/libjpeg.a  esp-idf/common_gpio/libcommon_gpio.a  esp-idf/camera/libcamera.a  esp-idf/espressif__esp_jpeg/libespressif__esp_jpeg.a  esp-idf/wifi_ap/libwifi_ap.a  -Wl,--start-group  /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/managed_components/espressif__esp_new_jpeg/lib/esp32s3/libesp_new_jpeg.a  -Wl,--end-group  esp-idf/spiffs/libspiffs.a  esp-idf/xtensa/libxtensa.a  esp-idf/esp_app_format/libesp_app_format.a  esp-idf/esp_bootloader_format/libesp_bootloader_format.a  esp-idf/app_update/libapp_update.a  esp-idf/esp_partition/libesp_partition.a  esp-idf/efuse/libefuse.a  esp-idf/esp_security/libesp_security.a  esp-idf/esp_driver_gpio/libesp_driver_gpio.a  esp-idf/esp_pm/libesp_pm.a  esp-idf/mbedtls/libmbedtls.a  esp-idf/bootloader_support/libbootloader_support.a  esp-idf/esp_mm/libesp_mm.a  esp-idf/spi_flash/libspi_flash.a  esp-idf/esp_system/libesp_system.a  esp-idf/esp_common/libesp_common.a  esp-idf/esp_rom/libesp_rom.a  esp-idf/hal/libhal.a  esp-idf/log/liblog.a  esp-idf/heap/libheap.a  esp-idf/soc/libsoc.a  esp-idf/esp_hw_support/libesp_hw_support.a  esp-idf/freertos/libfreertos.a  esp-idf/newlib/libnewlib.a  esp-idf/pthread/libpthread.a  esp-idf/cxx/libcxx.a  esp-idf/esp_timer/libesp_timer.a  esp-idf/esp_driver_gptimer/libesp_driver_gptimer.a  esp-idf/esp_ringbuf/libesp_ringbuf.a  esp-idf/esp_driver_uart/libesp_driver_uart.a  esp-idf/esp_event/libesp_event.a  esp-idf/nvs_flash/libnvs_flash.a  esp-idf/esp_phy/libesp_phy.a  esp-idf/esp_driver_usb_serial_jtag/libesp_driver_usb_serial_jtag.a  esp-idf/esp_vfs_console/libesp_vfs_console.a  esp-idf/vfs/libvfs.a  esp-idf/lwip/liblwip.a  esp-idf/esp_netif/libesp_netif.a  esp-idf/wpa_supplicant/libwpa_supplicant.a  esp-idf/esp_coex/libesp_coex.a  esp-idf/esp_wifi/libesp_wifi.a  esp-idf/esp_driver_spi/libesp_driver_spi.a  esp-idf/esp_gdbstub/libesp_gdbstub.a  esp-idf/esp_driver_pcnt/libesp_driver_pcnt.a  esp-idf/esp_driver_mcpwm/libesp_driver_mcpwm.a  esp-idf/esp_driver_i2s/libesp_driver_i2s.a  esp-idf/sdmmc/libsdmmc.a  esp-idf/esp_driver_sd_intf/libesp_driver_sd_intf.a  esp-idf/esp_driver_sdmmc/libesp_driver_sdmmc.a  esp-idf/esp_driver_sdspi/libesp_driver_sdspi.a  esp-idf/esp_driver_rmt/libesp_driver_rmt.a  esp-idf/esp_driver_tsens/libesp_driver_tsens.a  esp-idf/esp_driver_sdm/libesp_driver_sdm.a  esp-idf/esp_driver_i2c/libesp_driver_i2c.a  esp-idf/esp_driver_ledc/libesp_driver_ledc.a  esp-idf/esp_driver_twai/libesp_driver_twai.a  esp-idf/driver/libdriver.a  esp-idf/http_parser/libhttp_parser.a  esp-idf/esp-tls/libesp-tls.a  esp-idf/esp_adc/libesp_adc.a  esp-idf/esp_psram/libesp_psram.a  esp-idf/tcp_transport/libtcp_transport.a  esp-idf/esp_http_client/libesp_http_client.a  esp-idf/esp_http_server/libesp_http_server.a  esp-idf/esp_https_ota/libesp_https_ota.a  /Users/maochun/esp32prj/esp-idf/components/xtensa/esp32s3/libxt_hal.a  esp-idf/mbedtls/mbedtls/library/libmbedtls.a  esp-idf/mbedtls/mbedtls/library/libmbedcrypto.a  esp-idf/mbedtls/mbedtls/library/libmbedx509.a  esp-idf/mbedtls/mbedtls/3rdparty/everest/libeverest.a  esp-idf/mbedtls/mbedtls/3rdparty/p256-m/libp256m.a  /Users/maochun/esp32prj/esp-idf/components/esp_phy/lib/esp32s3/libbtbb.a  /Users/maochun/esp32prj/esp-idf/components/esp_phy/lib/esp32s3/libphy.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libcore.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libespnow.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libmesh.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libnet80211.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libpp.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libsmartconfig.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libwapi.a  esp-idf/xtensa/libxtensa.a  esp-idf/esp_app_format/libesp_app_format.a  esp-idf/esp_bootloader_format/libesp_bootloader_format.a  esp-idf/app_update/libapp_update.a  esp-idf/esp_partition/libesp_partition.a  esp-idf/efuse/libefuse.a  esp-idf/esp_security/libesp_security.a  esp-idf/esp_driver_gpio/libesp_driver_gpio.a  esp-idf/esp_pm/libesp_pm.a  esp-idf/mbedtls/libmbedtls.a  esp-idf/bootloader_support/libbootloader_support.a  esp-idf/esp_mm/libesp_mm.a  esp-idf/spi_flash/libspi_flash.a  esp-idf/esp_system/libesp_system.a  esp-idf/esp_common/libesp_common.a  esp-idf/esp_rom/libesp_rom.a  esp-idf/hal/libhal.a  esp-idf/log/liblog.a  esp-idf/heap/libheap.a  esp-idf/soc/libsoc.a  esp-idf/esp_hw_support/libesp_hw_support.a  esp-idf/freertos/libfreertos.a  esp-idf/newlib/libnewlib.a  esp-idf/pthread/libpthread.a  esp-idf/cxx/libcxx.a  esp-idf/esp_timer/libesp_timer.a  esp-idf/esp_driver_gptimer/libesp_driver_gptimer.a  esp-idf/esp_ringbuf/libesp_ringbuf.a  esp-idf/esp_driver_uart/libesp_driver_uart.a  esp-idf/esp_event/libesp_event.a  esp-idf/nvs_flash/libnvs_flash.a  esp-idf/esp_phy/libesp_phy.a  esp-idf/esp_driver_usb_serial_jtag/libesp_driver_usb_serial_jtag.a  esp-idf/esp_vfs_console/libesp_vfs_console.a  esp-idf/vfs/libvfs.a  esp-idf/lwip/liblwip.a  esp-idf/esp_netif/libesp_netif.a  esp-idf/wpa_supplicant/libwpa_supplicant.a  esp-idf/esp_coex/libesp_coex.a  esp-idf/esp_wifi/libesp_wifi.a  esp-idf/esp_driver_spi/libesp_driver_spi.a  esp-idf/esp_gdbstub/libesp_gdbstub.a  esp-idf/esp_driver_pcnt/libesp_driver_pcnt.a  esp-idf/esp_driver_mcpwm/libesp_driver_mcpwm.a  esp-idf/esp_driver_i2s/libesp_driver_i2s.a  esp-idf/sdmmc/libsdmmc.a  esp-idf/esp_driver_sd_intf/libesp_driver_sd_intf.a  esp-idf/esp_driver_sdmmc/libesp_driver_sdmmc.a  esp-idf/esp_driver_sdspi/libesp_driver_sdspi.a  esp-idf/esp_driver_rmt/libesp_driver_rmt.a  esp-idf/esp_driver_tsens/libesp_driver_tsens.a  esp-idf/esp_driver_sdm/libesp_driver_sdm.a  esp-idf/esp_driver_i2c/libesp_driver_i2c.a  esp-idf/esp_driver_ledc/libesp_driver_ledc.a  esp-idf/esp_driver_twai/libesp_driver_twai.a  esp-idf/driver/libdriver.a  esp-idf/http_parser/libhttp_parser.a  esp-idf/esp-tls/libesp-tls.a  esp-idf/esp_adc/libesp_adc.a  esp-idf/esp_psram/libesp_psram.a  esp-idf/tcp_transport/libtcp_transport.a  esp-idf/esp_http_client/libesp_http_client.a  esp-idf/esp_http_server/libesp_http_server.a  esp-idf/esp_https_ota/libesp_https_ota.a  /Users/maochun/esp32prj/esp-idf/components/xtensa/esp32s3/libxt_hal.a  esp-idf/mbedtls/mbedtls/library/libmbedtls.a  esp-idf/mbedtls/mbedtls/library/libmbedcrypto.a  esp-idf/mbedtls/mbedtls/library/libmbedx509.a  esp-idf/mbedtls/mbedtls/3rdparty/everest/libeverest.a  esp-idf/mbedtls/mbedtls/3rdparty/p256-m/libp256m.a  /Users/maochun/esp32prj/esp-idf/components/esp_phy/lib/esp32s3/libbtbb.a  /Users/maochun/esp32prj/esp-idf/components/esp_phy/lib/esp32s3/libphy.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libcore.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libespnow.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libmesh.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libnet80211.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libpp.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libsmartconfig.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libwapi.a  esp-idf/xtensa/libxtensa.a  esp-idf/esp_app_format/libesp_app_format.a  esp-idf/esp_bootloader_format/libesp_bootloader_format.a  esp-idf/app_update/libapp_update.a  esp-idf/esp_partition/libesp_partition.a  esp-idf/efuse/libefuse.a  esp-idf/esp_security/libesp_security.a  esp-idf/esp_driver_gpio/libesp_driver_gpio.a  esp-idf/esp_pm/libesp_pm.a  esp-idf/mbedtls/libmbedtls.a  esp-idf/bootloader_support/libbootloader_support.a  esp-idf/esp_mm/libesp_mm.a  esp-idf/spi_flash/libspi_flash.a  esp-idf/esp_system/libesp_system.a  esp-idf/esp_common/libesp_common.a  esp-idf/esp_rom/libesp_rom.a  esp-idf/hal/libhal.a  esp-idf/log/liblog.a  esp-idf/heap/libheap.a  esp-idf/soc/libsoc.a  esp-idf/esp_hw_support/libesp_hw_support.a  esp-idf/freertos/libfreertos.a  esp-idf/newlib/libnewlib.a  esp-idf/pthread/libpthread.a  esp-idf/cxx/libcxx.a  esp-idf/esp_timer/libesp_timer.a  esp-idf/esp_driver_gptimer/libesp_driver_gptimer.a  esp-idf/esp_ringbuf/libesp_ringbuf.a  esp-idf/esp_driver_uart/libesp_driver_uart.a  esp-idf/esp_event/libesp_event.a  esp-idf/nvs_flash/libnvs_flash.a  esp-idf/esp_phy/libesp_phy.a  esp-idf/esp_driver_usb_serial_jtag/libesp_driver_usb_serial_jtag.a  esp-idf/esp_vfs_console/libesp_vfs_console.a  esp-idf/vfs/libvfs.a  esp-idf/lwip/liblwip.a  esp-idf/esp_netif/libesp_netif.a  esp-idf/wpa_supplicant/libwpa_supplicant.a  esp-idf/esp_coex/libesp_coex.a  esp-idf/esp_wifi/libesp_wifi.a  esp-idf/esp_driver_spi/libesp_driver_spi.a  esp-idf/esp_gdbstub/libesp_gdbstub.a  esp-idf/esp_driver_pcnt/libesp_driver_pcnt.a  esp-idf/esp_driver_mcpwm/libesp_driver_mcpwm.a  esp-idf/esp_driver_i2s/libesp_driver_i2s.a  esp-idf/sdmmc/libsdmmc.a  esp-idf/esp_driver_sd_intf/libesp_driver_sd_intf.a  esp-idf/esp_driver_sdmmc/libesp_driver_sdmmc.a  esp-idf/esp_driver_sdspi/libesp_driver_sdspi.a  esp-idf/esp_driver_rmt/libesp_driver_rmt.a  esp-idf/esp_driver_tsens/libesp_driver_tsens.a  esp-idf/esp_driver_sdm/libesp_driver_sdm.a  esp-idf/esp_driver_i2c/libesp_driver_i2c.a  esp-idf/esp_driver_ledc/libesp_driver_ledc.a  esp-idf/esp_driver_twai/libesp_driver_twai.a  esp-idf/driver/libdriver.a  esp-idf/http_parser/libhttp_parser.a  esp-idf/esp-tls/libesp-tls.a  esp-idf/esp_adc/libesp_adc.a  esp-idf/esp_psram/libesp_psram.a  esp-idf/tcp_transport/libtcp_transport.a  esp-idf/esp_http_client/libesp_http_client.a  esp-idf/esp_http_server/libesp_http_server.a  esp-idf/esp_https_ota/libesp_https_ota.a  /Users/maochun/esp32prj/esp-idf/components/xtensa/esp32s3/libxt_hal.a  esp-idf/mbedtls/mbedtls/library/libmbedtls.a  esp-idf/mbedtls/mbedtls/library/libmbedcrypto.a  esp-idf/mbedtls/mbedtls/library/libmbedx509.a  esp-idf/mbedtls/mbedtls/3rdparty/everest/libeverest.a  esp-idf/mbedtls/mbedtls/3rdparty/p256-m/libp256m.a  /Users/maochun/esp32prj/esp-idf/components/esp_phy/lib/esp32s3/libbtbb.a  /Users/maochun/esp32prj/esp-idf/components/esp_phy/lib/esp32s3/libphy.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libcore.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libespnow.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libmesh.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libnet80211.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libpp.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libsmartconfig.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libwapi.a  esp-idf/xtensa/libxtensa.a  esp-idf/esp_app_format/libesp_app_format.a  esp-idf/esp_bootloader_format/libesp_bootloader_format.a  esp-idf/app_update/libapp_update.a  esp-idf/esp_partition/libesp_partition.a  esp-idf/efuse/libefuse.a  esp-idf/esp_security/libesp_security.a  esp-idf/esp_driver_gpio/libesp_driver_gpio.a  esp-idf/esp_pm/libesp_pm.a  esp-idf/mbedtls/libmbedtls.a  esp-idf/bootloader_support/libbootloader_support.a  esp-idf/esp_mm/libesp_mm.a  esp-idf/spi_flash/libspi_flash.a  esp-idf/esp_system/libesp_system.a  esp-idf/esp_common/libesp_common.a  esp-idf/esp_rom/libesp_rom.a  esp-idf/hal/libhal.a  esp-idf/log/liblog.a  esp-idf/heap/libheap.a  esp-idf/soc/libsoc.a  esp-idf/esp_hw_support/libesp_hw_support.a  esp-idf/freertos/libfreertos.a  esp-idf/newlib/libnewlib.a  esp-idf/pthread/libpthread.a  esp-idf/cxx/libcxx.a  esp-idf/esp_timer/libesp_timer.a  esp-idf/esp_driver_gptimer/libesp_driver_gptimer.a  esp-idf/esp_ringbuf/libesp_ringbuf.a  esp-idf/esp_driver_uart/libesp_driver_uart.a  esp-idf/esp_event/libesp_event.a  esp-idf/nvs_flash/libnvs_flash.a  esp-idf/esp_phy/libesp_phy.a  esp-idf/esp_driver_usb_serial_jtag/libesp_driver_usb_serial_jtag.a  esp-idf/esp_vfs_console/libesp_vfs_console.a  esp-idf/vfs/libvfs.a  esp-idf/lwip/liblwip.a  esp-idf/esp_netif/libesp_netif.a  esp-idf/wpa_supplicant/libwpa_supplicant.a  esp-idf/esp_coex/libesp_coex.a  esp-idf/esp_wifi/libesp_wifi.a  esp-idf/esp_driver_spi/libesp_driver_spi.a  esp-idf/esp_gdbstub/libesp_gdbstub.a  esp-idf/esp_driver_pcnt/libesp_driver_pcnt.a  esp-idf/esp_driver_mcpwm/libesp_driver_mcpwm.a  esp-idf/esp_driver_i2s/libesp_driver_i2s.a  esp-idf/sdmmc/libsdmmc.a  esp-idf/esp_driver_sd_intf/libesp_driver_sd_intf.a  esp-idf/esp_driver_sdmmc/libesp_driver_sdmmc.a  esp-idf/esp_driver_sdspi/libesp_driver_sdspi.a  esp-idf/esp_driver_rmt/libesp_driver_rmt.a  esp-idf/esp_driver_tsens/libesp_driver_tsens.a  esp-idf/esp_driver_sdm/libesp_driver_sdm.a  esp-idf/esp_driver_i2c/libesp_driver_i2c.a  esp-idf/esp_driver_ledc/libesp_driver_ledc.a  esp-idf/esp_driver_twai/libesp_driver_twai.a  esp-idf/driver/libdriver.a  esp-idf/http_parser/libhttp_parser.a  esp-idf/esp-tls/libesp-tls.a  esp-idf/esp_adc/libesp_adc.a  esp-idf/esp_psram/libesp_psram.a  esp-idf/tcp_transport/libtcp_transport.a  esp-idf/esp_http_client/libesp_http_client.a  esp-idf/esp_http_server/libesp_http_server.a  esp-idf/esp_https_ota/libesp_https_ota.a  /Users/maochun/esp32prj/esp-idf/components/xtensa/esp32s3/libxt_hal.a  esp-idf/mbedtls/mbedtls/library/libmbedtls.a  esp-idf/mbedtls/mbedtls/library/libmbedcrypto.a  esp-idf/mbedtls/mbedtls/library/libmbedx509.a  esp-idf/mbedtls/mbedtls/3rdparty/everest/libeverest.a  esp-idf/mbedtls/mbedtls/3rdparty/p256-m/libp256m.a  /Users/maochun/esp32prj/esp-idf/components/esp_phy/lib/esp32s3/libbtbb.a  /Users/maochun/esp32prj/esp-idf/components/esp_phy/lib/esp32s3/libphy.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libcore.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libespnow.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libmesh.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libnet80211.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libpp.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libsmartconfig.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libwapi.a  esp-idf/xtensa/libxtensa.a  esp-idf/esp_app_format/libesp_app_format.a  esp-idf/esp_bootloader_format/libesp_bootloader_format.a  esp-idf/app_update/libapp_update.a  esp-idf/esp_partition/libesp_partition.a  esp-idf/efuse/libefuse.a  esp-idf/esp_security/libesp_security.a  esp-idf/esp_driver_gpio/libesp_driver_gpio.a  esp-idf/esp_pm/libesp_pm.a  esp-idf/mbedtls/libmbedtls.a  esp-idf/bootloader_support/libbootloader_support.a  esp-idf/esp_mm/libesp_mm.a  esp-idf/spi_flash/libspi_flash.a  esp-idf/esp_system/libesp_system.a  esp-idf/esp_common/libesp_common.a  esp-idf/esp_rom/libesp_rom.a  esp-idf/hal/libhal.a  esp-idf/log/liblog.a  esp-idf/heap/libheap.a  esp-idf/soc/libsoc.a  esp-idf/esp_hw_support/libesp_hw_support.a  esp-idf/freertos/libfreertos.a  esp-idf/newlib/libnewlib.a  esp-idf/pthread/libpthread.a  esp-idf/cxx/libcxx.a  esp-idf/esp_timer/libesp_timer.a  esp-idf/esp_driver_gptimer/libesp_driver_gptimer.a  esp-idf/esp_ringbuf/libesp_ringbuf.a  esp-idf/esp_driver_uart/libesp_driver_uart.a  esp-idf/esp_event/libesp_event.a  esp-idf/nvs_flash/libnvs_flash.a  esp-idf/esp_phy/libesp_phy.a  esp-idf/esp_driver_usb_serial_jtag/libesp_driver_usb_serial_jtag.a  esp-idf/esp_vfs_console/libesp_vfs_console.a  esp-idf/vfs/libvfs.a  esp-idf/lwip/liblwip.a  esp-idf/esp_netif/libesp_netif.a  esp-idf/wpa_supplicant/libwpa_supplicant.a  esp-idf/esp_coex/libesp_coex.a  esp-idf/esp_wifi/libesp_wifi.a  esp-idf/esp_driver_spi/libesp_driver_spi.a  esp-idf/esp_gdbstub/libesp_gdbstub.a  esp-idf/esp_driver_pcnt/libesp_driver_pcnt.a  esp-idf/esp_driver_mcpwm/libesp_driver_mcpwm.a  esp-idf/esp_driver_i2s/libesp_driver_i2s.a  esp-idf/sdmmc/libsdmmc.a  esp-idf/esp_driver_sd_intf/libesp_driver_sd_intf.a  esp-idf/esp_driver_sdmmc/libesp_driver_sdmmc.a  esp-idf/esp_driver_sdspi/libesp_driver_sdspi.a  esp-idf/esp_driver_rmt/libesp_driver_rmt.a  esp-idf/esp_driver_tsens/libesp_driver_tsens.a  esp-idf/esp_driver_sdm/libesp_driver_sdm.a  esp-idf/esp_driver_i2c/libesp_driver_i2c.a  esp-idf/esp_driver_ledc/libesp_driver_ledc.a  esp-idf/esp_driver_twai/libesp_driver_twai.a  esp-idf/driver/libdriver.a  esp-idf/http_parser/libhttp_parser.a  esp-idf/esp-tls/libesp-tls.a  esp-idf/esp_adc/libesp_adc.a  esp-idf/esp_psram/libesp_psram.a  esp-idf/tcp_transport/libtcp_transport.a  esp-idf/esp_http_client/libesp_http_client.a  esp-idf/esp_http_server/libesp_http_server.a  esp-idf/esp_https_ota/libesp_https_ota.a  /Users/maochun/esp32prj/esp-idf/components/xtensa/esp32s3/libxt_hal.a  esp-idf/mbedtls/mbedtls/library/libmbedtls.a  esp-idf/mbedtls/mbedtls/library/libmbedcrypto.a  esp-idf/mbedtls/mbedtls/library/libmbedx509.a  esp-idf/mbedtls/mbedtls/3rdparty/everest/libeverest.a  esp-idf/mbedtls/mbedtls/3rdparty/p256-m/libp256m.a  /Users/maochun/esp32prj/esp-idf/components/esp_phy/lib/esp32s3/libbtbb.a  /Users/maochun/esp32prj/esp-idf/components/esp_phy/lib/esp32s3/libphy.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libcore.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libespnow.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libmesh.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libnet80211.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libpp.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libsmartconfig.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libwapi.a  esp-idf/xtensa/libxtensa.a  esp-idf/esp_app_format/libesp_app_format.a  esp-idf/esp_bootloader_format/libesp_bootloader_format.a  esp-idf/app_update/libapp_update.a  esp-idf/esp_partition/libesp_partition.a  esp-idf/efuse/libefuse.a  esp-idf/esp_security/libesp_security.a  esp-idf/esp_driver_gpio/libesp_driver_gpio.a  esp-idf/esp_pm/libesp_pm.a  esp-idf/mbedtls/libmbedtls.a  esp-idf/bootloader_support/libbootloader_support.a  esp-idf/esp_mm/libesp_mm.a  esp-idf/spi_flash/libspi_flash.a  esp-idf/esp_system/libesp_system.a  esp-idf/esp_common/libesp_common.a  esp-idf/esp_rom/libesp_rom.a  esp-idf/hal/libhal.a  esp-idf/log/liblog.a  esp-idf/heap/libheap.a  esp-idf/soc/libsoc.a  esp-idf/esp_hw_support/libesp_hw_support.a  esp-idf/freertos/libfreertos.a  esp-idf/newlib/libnewlib.a  esp-idf/pthread/libpthread.a  esp-idf/cxx/libcxx.a  esp-idf/esp_timer/libesp_timer.a  esp-idf/esp_driver_gptimer/libesp_driver_gptimer.a  esp-idf/esp_ringbuf/libesp_ringbuf.a  esp-idf/esp_driver_uart/libesp_driver_uart.a  esp-idf/esp_event/libesp_event.a  esp-idf/nvs_flash/libnvs_flash.a  esp-idf/esp_phy/libesp_phy.a  esp-idf/esp_driver_usb_serial_jtag/libesp_driver_usb_serial_jtag.a  esp-idf/esp_vfs_console/libesp_vfs_console.a  esp-idf/vfs/libvfs.a  esp-idf/lwip/liblwip.a  esp-idf/esp_netif/libesp_netif.a  esp-idf/wpa_supplicant/libwpa_supplicant.a  esp-idf/esp_coex/libesp_coex.a  esp-idf/esp_wifi/libesp_wifi.a  esp-idf/esp_driver_spi/libesp_driver_spi.a  esp-idf/esp_gdbstub/libesp_gdbstub.a  esp-idf/esp_driver_pcnt/libesp_driver_pcnt.a  esp-idf/esp_driver_mcpwm/libesp_driver_mcpwm.a  esp-idf/esp_driver_i2s/libesp_driver_i2s.a  esp-idf/sdmmc/libsdmmc.a  esp-idf/esp_driver_sd_intf/libesp_driver_sd_intf.a  esp-idf/esp_driver_sdmmc/libesp_driver_sdmmc.a  esp-idf/esp_driver_sdspi/libesp_driver_sdspi.a  esp-idf/esp_driver_rmt/libesp_driver_rmt.a  esp-idf/esp_driver_tsens/libesp_driver_tsens.a  esp-idf/esp_driver_sdm/libesp_driver_sdm.a  esp-idf/esp_driver_i2c/libesp_driver_i2c.a  esp-idf/esp_driver_ledc/libesp_driver_ledc.a  esp-idf/esp_driver_twai/libesp_driver_twai.a  esp-idf/driver/libdriver.a  esp-idf/http_parser/libhttp_parser.a  esp-idf/esp-tls/libesp-tls.a  esp-idf/esp_adc/libesp_adc.a  esp-idf/esp_psram/libesp_psram.a  esp-idf/tcp_transport/libtcp_transport.a  esp-idf/esp_http_client/libesp_http_client.a  esp-idf/esp_http_server/libesp_http_server.a  esp-idf/esp_https_ota/libesp_https_ota.a  /Users/maochun/esp32prj/esp-idf/components/xtensa/esp32s3/libxt_hal.a  esp-idf/mbedtls/mbedtls/library/libmbedtls.a  esp-idf/mbedtls/mbedtls/library/libmbedcrypto.a  esp-idf/mbedtls/mbedtls/library/libmbedx509.a  esp-idf/mbedtls/mbedtls/3rdparty/everest/libeverest.a  esp-idf/mbedtls/mbedtls/3rdparty/p256-m/libp256m.a  /Users/maochun/esp32prj/esp-idf/components/esp_phy/lib/esp32s3/libbtbb.a  /Users/maochun/esp32prj/esp-idf/components/esp_phy/lib/esp32s3/libphy.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libcore.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libespnow.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libmesh.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libnet80211.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libpp.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libsmartconfig.a  /Users/maochun/esp32prj/esp-idf/components/esp_wifi/lib/esp32s3/libwapi.a  -u esp_app_desc  -u esp_efuse_startup_include_func  -u esp_security_init_include_impl  -u ld_include_highint_hdl  -u start_app  -u start_app_other_cores  -u __ubsan_include  -u esp_system_include_startup_funcs  -Wl,--wrap=longjmp  -u __assert_func  -u esp_sleep_gpio_include  -Wl,--undefined=FreeRTOS_openocd_params  -u app_main  -lc  -lm  -u esp_libc_include_heap_impl  -u esp_libc_include_reent_syscalls_impl  -u esp_libc_include_syscalls_impl  -u esp_libc_include_pthread_impl  -u esp_libc_include_assert_impl  -u esp_libc_include_getentropy_impl  -u esp_libc_include_init_funcs  -u esp_libc_init_funcs  -u pthread_include_pthread_impl  -u pthread_include_pthread_cond_var_impl  -u pthread_include_pthread_local_storage_impl  -u pthread_include_pthread_rwlock_impl  -u pthread_include_pthread_semaphore_impl  -Wl,--wrap=__register_frame_info_bases  -Wl,--wrap=__register_frame_info  -Wl,--wrap=__register_frame  -Wl,--wrap=__register_frame_info_table_bases  -Wl,--wrap=__register_frame_info_table  -Wl,--wrap=__register_frame_table  -Wl,--wrap=__deregister_frame_info_bases  -Wl,--wrap=__deregister_frame_info  -Wl,--wrap=_Unwind_Find_FDE  -Wl,--wrap=_Unwind_GetGR  -Wl,--wrap=_Unwind_GetCFA  -Wl,--wrap=_Unwind_GetIP  -Wl,--wrap=_Unwind_GetIPInfo  -Wl,--wrap=_Unwind_GetRegionStart  -Wl,--wrap=_Unwind_GetDataRelBase  -Wl,--wrap=_Unwind_GetTextRelBase  -Wl,--wrap=_Unwind_SetIP  -Wl,--wrap=_Unwind_SetGR  -Wl,--wrap=_Unwind_GetLanguageSpecificData  -Wl,--wrap=_Unwind_FindEnclosingFunction  -Wl,--wrap=_Unwind_Resume  -Wl,--wrap=_Unwind_RaiseException  -Wl,--wrap=_Unwind_DeleteException  -Wl,--wrap=_Unwind_ForcedUnwind  -Wl,--wrap=_Unwind_Resume_or_Rethrow  -Wl,--wrap=_Unwind_Backtrace  -Wl,--wrap=__cxa_call_unexpected  -Wl,--wrap=__gxx_personality_v0  -Wl,--wrap=__cxa_throw  -Wl,--wrap=__cxa_allocate_exception  -lstdc++  esp-idf/pthread/libpthread.a  esp-idf/newlib/libnewlib.a  -u __cxa_guard_dummy  -u __cxx_init_dummy  -lgcc  esp-idf/cxx/libcxx.a  -u __cxx_fatal_exception  -u esp_timer_init_include_func  -u uart_vfs_include_dev_init  -u usb_serial_jtag_vfs_include_dev_init  -u usb_serial_jtag_connection_monitor_include  -u esp_vfs_include_console_register  -u vfs_include_syscalls_impl  -u esp_vfs_include_nullfs_register && :
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: esp-idf/motor_handler/libmotor_handler.a(motor_handler.c.obj):(.literal.motor_handler_update+0x1c): undefined reference to `pwm_stop_all_motors'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: esp-idf/motor_handler/libmotor_handler.a(motor_handler.c.obj):(.literal.motor_handler_update+0x20): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: esp-idf/motor_handler/libmotor_handler.a(motor_handler.c.obj):(.literal.motor_handler_update+0x24): undefined reference to `pwm_set_motor_speed'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: esp-idf/motor_handler/libmotor_handler.a(motor_handler.c.obj): in function `motor_handler_update':
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:40:(.text.motor_handler_update+0x6): undefined reference to `pwm_stop_all_motors'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:46:(.text.motor_handler_update+0x32): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:47:(.text.motor_handler_update+0x3c): undefined reference to `pwm_set_motor_speed'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:54:(.text.motor_handler_update+0x70): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:55:(.text.motor_handler_update+0x7a): undefined reference to `pwm_set_motor_speed'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:61:(.text.motor_handler_update+0xaa): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:62:(.text.motor_handler_update+0xb4): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:63:(.text.motor_handler_update+0xbe): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:64:(.text.motor_handler_update+0xc8): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:66:(.text.motor_handler_update+0xd8): undefined reference to `pwm_set_motor_speed'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:72:(.text.motor_handler_update+0x108): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:73:(.text.motor_handler_update+0x112): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:74:(.text.motor_handler_update+0x11c): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:75:(.text.motor_handler_update+0x126): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:77:(.text.motor_handler_update+0x136): undefined reference to `pwm_set_motor_speed'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:84:(.text.motor_handler_update+0x162): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:85:(.text.motor_handler_update+0x16c): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:86:(.text.motor_handler_update+0x176): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:87:(.text.motor_handler_update+0x180): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:89:(.text.motor_handler_update+0x190): undefined reference to `pwm_set_motor_speed'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:95:(.text.motor_handler_update+0x1c0): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:96:(.text.motor_handler_update+0x1ca): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:97:(.text.motor_handler_update+0x1d4): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:98:(.text.motor_handler_update+0x1de): undefined reference to `pwm_set_motor_direction'
/Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/../lib/gcc/xtensa-esp-elf/15.1.0/../../../../xtensa-esp-elf/bin/ld: /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:100:(.text.motor_handler_update+0x1ee): undefined reference to `pwm_set_motor_speed'
collect2: error: ld returned 1 exit status
ninja: build stopped: subcommand failed.
ninja failed with exit code 1, output of the command is in the /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/build/log/idf_py_stderr_output_28502 and /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/build/log/idf_py_stdout_output_28502
MMMBP:esp32_wifi_control maochun$ 



motor_handler.c error
ndler.dir/motor_handler.c.obj.d -o esp-idf/motor_handler/CMakeFiles/__idf_motor_handler.dir/motor_handler.c.obj -c /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:39:5: warning: data definition has no type or storage class
   39 |     pwm_stop_all_motors();
      |     ^~~~~~~~~~~~~~~~~~~
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:39:5: error: type defaults to 'int' in declaration of 'pwm_stop_all_motors' [-Wimplicit-int]
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:39:5: error: conflicting types for 'pwm_stop_all_motors'; have 'int(void)'
In file included from /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:5:
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/components/motor_pwm/motor_pwm.h:30:6: note: previous declaration of 'pwm_stop_all_motors' with type 'void(void)'
   30 | void pwm_stop_all_motors(void);
      |      ^~~~~~~~~~~~~~~~~~~
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:42:5: error: expected identifier or '(' before 'if'
   42 |     if (cmd->forward) {
      |     ^~
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:50:5: error: expected identifier or '(' before 'else'
   50 |     else if (cmd->backward) {
      |     ^~~~
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:58:5: error: expected identifier or '(' before 'else'
   58 |     else if (cmd->right) {
      |     ^~~~
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:69:5: error: expected identifier or '(' before 'else'
   69 |     else if (cmd->left) {
      |     ^~~~
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:81:5: error: expected identifier or '(' before 'if'
   81 |     if (cmd->crotator) {
      |     ^~
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:92:5: error: expected identifier or '(' before 'else'
   92 |     else if (cmd->acrotator) {
      |     ^~~~
/Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/handler/motor_handler/motor_handler.c:102:1: error: expected identifier or '(' before '}' token
  102 | }
      | ^
[909/1093] Performing configure step for 'bootloader'
-- Found Git: /usr/bin/git (found version "2.37.1 (Apple Git-137.1)")
-- Minimal build - OFF
-- The C compiler identification is GNU 15.1.0
-- The CXX compiler identification is GNU 15.1.0
-- The ASM compiler identification is GNU
-- Found assembler: /Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/xtensa-esp32s3-elf-gcc
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/xtensa-esp32s3-elf-gcc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /Users/maochun/.espressif/tools/xtensa-esp-elf/esp-15.1.0_20250607/xtensa-esp-elf/bin/xtensa-esp32s3-elf-g++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Building ESP-IDF components for target esp32s3
-- ESP-TEE is currently supported only on the esp32c6;esp32h2;esp32c5 SoCs
-- Project sdkconfig file /Users/maochun/esp32prj/Project_CAM/branch/esp32_wifi_control/sdkconfig
-- Compiler supported targets: xtensa-esp-elf
-- Adding linker script /Users/maochun/esp32prj/esp-idf/components/soc/esp32s3/ld/esp32s3.peripherals.ld
-- Bootloader project name: "bootloader" version: 1
-- Adding linker script /Users/maochun/esp32prj/esp-idf/components/esp_rom/esp32s3/ld/esp32s3.rom.ld
-- Adding linker script /Users/maochun/esp32prj/esp-idf/components/esp_rom/esp32s3/ld/esp32s3.rom.api.ld
-- Adding linker script /Users


only have web control log
I (29127) webserver_control: Direction: , State: 0
I (29187) webserver_control: HTTP GET /control
I (29187) webserver_control: Direction: , State: 0
I (29247) webserver_control: HTTP GET /control
I (29247) webserver_control: Direction: , State: 0
I (29307) webserver_control: HTTP GET /control
I (29307) webserver_control: Direction: , State: 0
I (29377) webserver_control: HTTP GET /control
I (29377) webserver_control: Direction: , State: 0
I (29437) webserver_control: HTTP GET /control
I (29437) webserver_control: Direction: , State: 0
I (29497) webserver_control: HTTP GET /control
I (29497) webserver_control: Direction: , State: 0
I (29557) webserver_control: HTTP GET /control
I (29557) webserver_control: Direction: , State: 0
I (29627) webserver_control: HTTP GET /control
I (29627) webserver_control: Direction: , State: 0
I (29687) webserver_control: HTTP GET /control
I (29687) webserver_control: Direction: , State: 0
I (29757) webserver_control: HTTP GET /control


I (1066) wifi_ap: DHCP server is running.
I (1066) PSRAM: PSRAM size: 8388608 bytes
I (1076) PSRAM: Free heap: 8660428 bytes
I (1076) PSRAM: PSRAM is initialized and ready.
I (1076) PSRAM: Successfully allocated 1KB from PSRAM.
I (1086) CAMERA: Initializing camera sensor...
I (1086) VSYNC_CHECK: GPIO9 level: 0
I (1096) SCCB: i2c_param_config returned: 0 (ESP_OK)
I (1096) SCCB: i2c_driver_install returned: 0 (ESP_OK)
E (1106) SCCB: Write failed: reg=0x12, data=0x80
I (1116) CAMERA: COM10 = 0x00
I (1116) VSYNC_DEBUG: VSYNC GPIO9: High=0, Low=100
E (1116) SCCB: Write failed: reg=0x11, data=0x80
E (1116) CAMERA: Failed to write register 0x11
E (1126) CAMERA: Failed to configure OV7670 registers
E (1126) SCCB: Write failed: reg=0x11, data=0x80
E (1136) CAMERA: Failed to write register 0x11
I (1226) SPIFFS:: SPIFFS mounted successfully
I (1226) webserver: Starting webserver...
I (1226) webserver: Webserver started
系统初始化完成，等待客户端连接...
I (1226) main_task: Returned from app_main()
I (2116) VSYNC_DEBUG: VSYNC GPIO9: High=0, Low=100
I (3116) VSYNC_DEBUG: VSYNC GPIO9: High=0, Low=100
I (4116) VSYNC_DEBUG: VSYNC GPIO9: High=0, Low=100
I (5116) VSYNC_DEBUG: VSYNC GPIO9: High=0, Low=100
I (6116) VSYNC_DEBUG: VSYNC GPIO9: High=0, Low=100
I (7116) VSYNC_DEBUG: VSYNC GPIO9: High=0, Low=100
I (8116) VSYNC_DEBUG: VSYNC GPIO9: High=0, Low=100
I (9116) VSYNC_DEBUG: VSYNC GPIO9: High=0, Low=100
I (10116) VSYNC_DEBUG: VSYNC GPIO9: High=0, Low=100

Done
MMMBP:esp32_wifi_control maochun$ 



2025-09-29:
sccb protocol issue
I (1136) CAMERA: Initializing camera sensor...
I (1146) VSYNC_CHECK: GPIO21 level: 0
I (1156) CAMERA: COM10 = 0x00
I (1156) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
E (1156) SCCB: Write failed: reg=0x11, data=0x80
E (1156) CAMERA: Failed to write register 0x11
E (1166) CAMERA: Failed to configure OV7670 registers
E (1166) SCCB: Write failed: reg=0x11, data=0x80
E (1176) CAMERA: Failed to write register 0x11
I (1266) SPIFFS:: SPIFFS mounted successfully
I (1266) webserver: Starting webserver...
I (1266) webserver: Webserver started
系统初始化完成，等待客户端连接...
I (1266) main_task: Returned from app_main()
I (2156) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (3156) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (4156) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (5156) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (6156) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (7156) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (7186) wifi:new:<1,1>, old:<1,1>, ap:<1,1>, sta:<255,255>, prof:1, snd_ch_cfg:0x0
I (7196) wifi:station: 5c:5f:67:ed:16:1a join, AID=1, bgn, 40U
I (7226) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (8156) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100




I (1067) PSRAM: PSRAM size: 8388608 bytes
I (1077) PSRAM: Free heap: 8660428 bytes
I (1077) PSRAM: PSRAM is initialized and ready.
I (1087) PSRAM: Successfully allocated 1KB from PSRAM.
I (1087) camera: : Initializing camera sensor...
I (1087) VSYNC_CHECK: GPIO21 level: 0
I (1097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
W (1107) camera: : VSYNC signal not detected
I (1107) camera: : Camera sensor initialization complete.
I (1197) SPIFFS:: SPIFFS mounted successfully
I (1207) webserver: Starting webserver...
I (1207) webserver: Webserver started
系统初始化完成，等待客户端连接...
I (1207) main_task: Returned from app_main()
I (2097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (3097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (4097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (5097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (6097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (6777) wifi:new:<1,1>, old:<1,1>, ap:<1,1>, sta:<255,255>, prof:1, snd_ch_cfg:0x0
I (6777) wifi:station: 5c:5f:67:ed:16:1a join, AID=1, bgn, 40U
I (6827) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (7097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (8097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (8537) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (9097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (10097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (11097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (12097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (13097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (14097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (14847) wifi:<ba-add>idx:2 (ifx:1, 5c:5f:67:ed:16:1a), tid:0, ssn:76, winSize:64
I (15097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (16097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (17097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (18097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (19097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (20097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (21097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (22097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (23097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (24097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (25097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (26097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (27097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (27837) webserver_control: HTTP POST /toggle
I (27837) webserver_control: Received toggle command: ON
I (28097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (29097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (30097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (30627) webserver_camera: HTTP GET /image
I (30627) webserver_camera: Allocating frame buffer (153600 bytes)
I (30637) webserver_camera: Free heap before alloc: 8648272
I (30637) OV7670_HANDLER: Reading raw frame of size: 153600
I (30637) OV7670_HANDLER: Resetting FIFO read pointer...
I (30647) OV7670_HANDLER: Enabling FIFO output...
I (30647) OV7670_HANDLER: Byte[0] = 0xEF
I (30657) OV7670_HANDLER: Byte[1] = 0x89
I (30657) OV7670_HANDLER: Byte[2] = 0xF3
I (30657) OV7670_HANDLER: Byte[3] = 0x86
I (31097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (31247) OV7670_HANDLER: Disabling FIFO output...
I (31247) OV7670_HANDLER: Raw frame read complete
I (31247) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (31247) jpeg: Allocating RGB888 buffer: 230400 bytes
I (31257) jpeg: Converting RGB565 to RGB888...
I (31257) jpeg: First pixel RGB565: 0x89EF → RGB888: R=136 G=60 B=120
I (31277) jpeg: Starting JPEG encoding...
W (31287) jpeg: JPEG buffer expanded to 307200 bytes
I (31297) jpeg: JPEG encoding successful: 230422 bytes
I (31297) webserver_camera: JPEG encoded size: 230422 bytes
I (32097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (33097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (34097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (35097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (36097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (37097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (38097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (39097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (40097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (41097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100
I (42097) VSYNC_DEBUG: VSYNC GPIO21: High=0, Low=100





update web structure, VSYNC still not 
ESP32S3 Boot Success...
I (916) pp: pp rom version: e7ae62f
I (916) net80211: net80211 rom version: e7ae62f
I (926) wifi:wifi driver task: 3fca7fa0, prio:23, stack:6656, core=0
I (926) wifi:wifi firmware version: df85326
I (926) wifi:wifi certification version: v7.0
I (936) wifi:config NVS flash: enabled
I (936) wifi:config nano formatting: disabled
I (936) wifi:Init data frame dynamic rx buffer num: 32
I (936) wifi:Init static rx mgmt buffer num: 5
I (946) wifi:Init management short buffer num: 32
I (946) wifi:Init dynamic tx buffer num: 32
I (956) wifi:Init static tx FG buffer num: 2
I (956) wifi:Init static rx buffer size: 1600
I (956) wifi:Init static rx buffer num: 10
I (966) wifi:Init dynamic rx buffer num: 32
I (966) wifi_init: rx ba win: 6
I (976) wifi_init: accept mbox: 6
I (976) wifi_init: tcpip mbox: 32
I (976) wifi_init: udp mbox: 6
I (976) wifi_init: tcp mbox: 6
I (986) wifi_init: tcp tx win: 5760
I (986) wifi_init: tcp rx win: 5760
I (986) wifi_init: tcp mss: 1440
I (996) wifi_init: WiFi IRAM OP enabled
I (996) wifi_init: WiFi RX IRAM OP enabled
I (1006) phy_init: phy_version 701,f4f1da3a,Mar  3 2025,15:50:10
I (1046) phy_init: Saving new calibration data due to checksum failure or outdated calibration data, mode(0)
I (1106) wifi:mode : softAP (dc:b4:d9:04:7c:05)
I (1106) wifi:Total power save buffer number: 16
I (1106) wifi:Init max length of beacon: 752/752
I (1106) wifi:Init max length of beacon: 752/752
I (1106) wifi_ap: WiFi SoftAP started. SSID: ESP32-TEST, Password: niwenwoa
I (1106) esp_netif_lwip: DHCP server started on interface WIFI_AP_DEF with IP: 192.168.4.1
I (1126) wifi_ap: DHCP server is running.
I (1126) PSRAM: PSRAM size: 8388608 bytes
I (1126) PSRAM: Free heap: 8660428 bytes
I (1136) PSRAM: PSRAM is initialized and ready.
I (1136) PSRAM: Successfully allocated 1KB from PSRAM.
I (1146) camera: : Initializing camera sensor...
W (1186) camera: : VSYNC signal not detected
I (1186) camera: : Camera sensor initialization complete.
I (1276) SPIFFS:: SPIFFS mounted successfully
I (1276) webserver: Starting webserver...
I (1276) webserver: Webserver started
系统初始化完成，等待客户端连接...
I (1276) main_task: Returned from app_main()
I (6386) wifi:new:<1,1>, old:<1,1>, ap:<1,1>, sta:<255,255>, prof:1, snd_ch_cfg:0x0
I (6386) wifi:station: 5c:5f:67:ed:16:1a join, AID=1, bgn, 40U
I (6416) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (7736) webserver: HTTP GET /
I (9536) wifi:<ba-add>idx:2 (ifx:1, 5c:5f:67:ed:16:1a), tid:0, ssn:71, winSize:64
I (13196) webserver_control: HTTP POST /toggle
I (13196) webserver_control: Received toggle command: ON
I (14756) webserver_control: HTTP POST /toggle
I (14756) webserver_control: Received toggle command: ON
I (17026) webserver_camera: HTTP GET /image
I (17026) webserver_camera: Allocating frame buffer (153600 bytes)
I (17026) webserver_camera: Free heap before alloc: 8650680
I (17026) OV7670_HANDLER: Reading raw frame of size: 153600
I (17036) OV7670_HANDLER: Resetting FIFO read pointer...
I (17036) OV7670_HANDLER: Enabling FIFO output...
I (17046) OV7670_HANDLER: Byte[0] = 0xED
I (17046) OV7670_HANDLER: Byte[1] = 0xED
I (17046) OV7670_HANDLER: Byte[2] = 0xED
I (17056) OV7670_HANDLER: Byte[3] = 0x01
I (17636) OV7670_HANDLER: Disabling FIFO output...
I (17636) OV7670_HANDLER: Raw frame read complete
I (17636) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (17646) jpeg: Allocating RGB888 buffer: 230400 bytes
I (17646) jpeg: Converting RGB565 to RGB888...
I (17646) jpeg: First pixel RGB565: 0xEDED → RGB888: R=232 G=188 B=104
I (17676) jpeg: Starting JPEG encoding...
W (17676) jpeg: JPEG buffer expanded to 307200 bytes
I (17686) jpeg: JPEG encoding successful: 230422 bytes
I (17686) webserver_camera: JPEG encoded size: 230422 bytes
I (30236) webserver_control: HTTP GET /control
I (30236) webserver_control: Direction: forward, State: 1
I (30376) webserver_control: HTTP GET /control
I (30376) webserver_control: Direction: forward, State: 0
I (32966) webserver_control: HTTP GET /control
I (32966) webserver_control: Direction: forward, State: 1
I (33886) webserver_control: HTTP GET /control
I (33896) webserver_control: Direction: forward, State: 0
I (34676) webserver_control: HTTP GET /control
I (34686) webserver_control: Direction: forward, State: 1
I (34816) webserver_control: HTTP GET /control
I (34816) webserver_control: Direction: forward, State: 0
I (37096) webserver_control: HTTP GET /control
I (37096) webserver_control: Direction: forward, State: 1
I (37256) webserver_control: HTTP GET /control
I (37256) webserver_control: Direction: forward, State: 0
I (39206) webserver_control: HTTP GET /control
I (39206) webserver_control: Direction: left, State: 1
I (39346) webserver_control: HTTP GET /control
I (39346) webserver_control: Direction: left, State: 0
I (40086) webserver_control: HTTP GET /control
I (40086) webserver_control: Direction: left, State: 1
I (40266) webserver_control: HTTP GET /control
I (40266) webserver_control: Direction: left, State: 0
I (42966) webserver_control: HTTP GET /control
I (42966) webserver_control: Direction: right, State: 1
I (43036) webserver_control: HTTP GET /control
I (43036) webserver_control: Direction: right, State: 0
I (43686) webserver_control: HTTP GET /control
I (43686) webserver_control: Direction: right, State: 1
I (43806) webserver_control: HTTP GET /control
I (43806) webserver_control: Direction: right, State: 0
I (44026) webserver_control: HTTP GET /control
I (44026) webserver_control: Direction: right, State: 1
I (44126) webserver_control: HTTP GET /control
I (44126) webserver_control: Direction: right, State: 0
I (44256) webserver_control: HTTP GET /control
I (44256) webserver_control: Direction: right, State: 1
I (44326) webserver_control: HTTP GET /control
I (44336) webserver_control: Direction: right, State: 0
I (46076) webserver_control: HTTP GET /control
I (46076) webserver_control: Direction: backward, State: 1
I (46636) webserver_control: HTTP GET /control
I (46636) webserver_control: Direction: backward, State: 0
I (47486) webserver_control: HTTP GET /control
I (47486) webserver_control: Direction: backward, State: 1
I (48456) webserver_control: HTTP GET /control
I (48466) webserver_control: Direction: backward, State: 0



change VSYNC to GPIO21, no warning print:
ESP32S3 Boot Success...
I (917) pp: pp rom version: e7ae62f
I (917) net80211: net80211 rom version: e7ae62f
I (927) wifi:wifi driver task: 3fca7fa0, prio:23, stack:6656, core=0
I (927) wifi:wifi firmware version: df85326
I (927) wifi:wifi certification version: v7.0
I (927) wifi:config NVS flash: enabled
I (927) wifi:config nano formatting: disabled
I (937) wifi:Init data frame dynamic rx buffer num: 32
I (937) wifi:Init static rx mgmt buffer num: 5
I (947) wifi:Init management short buffer num: 32
I (947) wifi:Init dynamic tx buffer num: 32
I (957) wifi:Init static tx FG buffer num: 2
I (957) wifi:Init static rx buffer size: 1600
I (957) wifi:Init static rx buffer num: 10
I (967) wifi:Init dynamic rx buffer num: 32
I (967) wifi_init: rx ba win: 6
I (967) wifi_init: accept mbox: 6
I (977) wifi_init: tcpip mbox: 32
I (977) wifi_init: udp mbox: 6
I (977) wifi_init: tcp mbox: 6
I (987) wifi_init: tcp tx win: 5760
I (987) wifi_init: tcp rx win: 5760
I (987) wifi_init: tcp mss: 1440
I (997) wifi_init: WiFi IRAM OP enabled
I (997) wifi_init: WiFi RX IRAM OP enabled
I (1007) phy_init: phy_version 701,f4f1da3a,Mar  3 2025,15:50:10
I (1047) wifi:mode : softAP (dc:b4:d9:04:7c:05)
I (1057) wifi:Total power save buffer number: 16
I (1057) wifi:Init max length of beacon: 752/752
I (1057) wifi:Init max length of beacon: 752/752
I (1057) wifi_ap: WiFi SoftAP started. SSID: ESP32-TEST, Password: niwenwoa
I (1057) esp_netif_lwip: DHCP server started on interface WIFI_AP_DEF with IP: 192.168.4.1
I (1067) wifi_ap: DHCP server is running.
I (1077) PSRAM: PSRAM size: 8388608 bytes
I (1077) PSRAM: Free heap: 8660224 bytes
I (1087) PSRAM: PSRAM is initialized and ready.
I (1087) PSRAM: Successfully allocated 1KB from PSRAM.
I (1087) camera: : Initializing camera sensor...
I (1097) camera: : Camera sensor initialization complete.
I (1187) SPIFFS:: SPIFFS mounted successfully
I (1187) webserver: Starting webserver...
I (1197) webserver: Webserver started
系统初始化完成，等待客户端连接...
I (1197) main_task: Returned from app_main()
I (6617) wifi:new:<1,1>, old:<1,1>, ap:<1,1>, sta:<255,255>, prof:1, snd_ch_cfg:0x0
I (6617) wifi:station: 5c:5f:67:ed:16:1a join, AID=1, bgn, 40U
I (6667) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (9637) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (9727) webserver: HTTP GET /
I (9787) webserver: HTTP GET /favicon.ico
I (12767) webserver: HTTP POST /toggle
I (12767) webserver: Received toggle command: ON
I (15987) webserver: HTTP GET /image
I (15987) webserver: Allocating frame buffer (153600 bytes)
I (15987) webserver: Free heap before alloc: 8650924
I (15987) OV7670_HANDLER: Reading raw frame of size: 153600
I (15987) OV7670_HANDLER: Resetting FIFO read pointer...
I (15997) OV7670_HANDLER: Enabling FIFO output...
I (15997) OV7670_HANDLER: Byte[0] = 0x62
I (16007) OV7670_HANDLER: Byte[1] = 0x02
I (16007) OV7670_HANDLER: Byte[2] = 0x02
I (16007) OV7670_HANDLER: Byte[3] = 0xEE
I (16597) OV7670_HANDLER: Disabling FIFO output...
I (16597) OV7670_HANDLER: Raw frame read complete
I (16597) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (16597) jpeg: Allocating RGB888 buffer: 230400 bytes
I (16607) jpeg: Converting RGB565 to RGB888...
I (16607) jpeg: First pixel RGB565: 0x0262 → RGB888: R=0 G=76 B=16
I (16637) jpeg: Starting JPEG encoding...
W (16637) jpeg: JPEG buffer expanded to 307200 bytes
I (16647) jpeg: JPEG encoding successful: 230422 bytes
I (16647) webserver: JPEG encoded size: 230422 bytes
I (16817) wifi:<ba-add>idx:2 (ifx:1, 5c:5f:67:ed:16:1a), tid:0, ssn:145, winSize:64
I (63027) webserver: HTTP GET /image
I (63027) webserver: Allocating frame buffer (153600 bytes)
I (63027) webserver: Free heap before alloc: 8650844
I (63037) OV7670_HANDLER: Reading raw frame of size: 153600
I (63037) OV7670_HANDLER: Resetting FIFO read pointer...
I (63047) OV7670_HANDLER: Enabling FIFO output...
I (63047) OV7670_HANDLER: Byte[0] = 0x02
I (63047) OV7670_HANDLER: Byte[1] = 0xED
I (63057) OV7670_HANDLER: Byte[2] = 0xED
I (63057) OV7670_HANDLER: Byte[3] = 0x01
I (63647) OV7670_HANDLER: Disabling FIFO output...
I (63647) OV7670_HANDLER: Raw frame read complete
I (63647) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (63647) jpeg: Allocating RGB888 buffer: 230400 bytes
I (63657) jpeg: Converting RGB565 to RGB888...
I (63657) jpeg: First pixel RGB565: 0xED02 → RGB888: R=232 G=160 B=16
I (63677) jpeg: Starting JPEG encoding...
W (63687) jpeg: JPEG buffer expanded to 307200 bytes
I (63697) jpeg: JPEG encoding successful: 230422 bytes
I (63697) webserver: JPEG encoded size: 230422 bytes


update init sequence
I (848) spi_flash: flash io: dio
W (851) spi_flash: Detected size(16384k) larger than the size in the binary image header(8192k). Using the size in the binary image header.
W (864) i2c: This driver is an old driver, please migrate your application code to adapt `driver/i2c_master.h`
I (873) sleep_gpio: Configure to isolate all GPIO pins in sleep state
I (880) sleep_gpio: Enable automatic switching of GPIO sleep configuration
I (887) main_task: Started on CPU0
I (897) main_task: Calling app_main()
ESP32S3 Boot Success...
I (917) pp: pp rom version: e7ae62f
I (917) net80211: net80211 rom version: e7ae62f
I (927) wifi:wifi driver task: 3fca7fa0, prio:23, stack:6656, core=0
I (927) wifi:wifi firmware version: df85326
I (927) wifi:wifi certification version: v7.0
I (927) wifi:config NVS flash: enabled
I (927) wifi:config nano formatting: disabled
I (937) wifi:Init data frame dynamic rx buffer num: 32
I (937) wifi:Init static rx mgmt buffer num: 5
I (947) wifi:Init management short buffer num: 32
I (947) wifi:Init dynamic tx buffer num: 32
I (957) wifi:Init static tx FG buffer num: 2
I (957) wifi:Init static rx buffer size: 1600
I (957) wifi:Init static rx buffer num: 10
I (967) wifi:Init dynamic rx buffer num: 32
I (967) wifi_init: rx ba win: 6
I (967) wifi_init: accept mbox: 6
I (977) wifi_init: tcpip mbox: 32
I (977) wifi_init: udp mbox: 6
I (977) wifi_init: tcp mbox: 6
I (987) wifi_init: tcp tx win: 5760
I (987) wifi_init: tcp rx win: 5760
I (987) wifi_init: tcp mss: 1440
I (997) wifi_init: WiFi IRAM OP enabled
I (997) wifi_init: WiFi RX IRAM OP enabled
I (1007) phy_init: phy_version 701,f4f1da3a,Mar  3 2025,15:50:10
I (1047) phy_init: Saving new calibration data due to checksum failure or outdated calibration data, mode(0)
I (1107) wifi:mode : softAP (dc:b4:d9:04:7c:05)
I (1117) wifi:Total power save buffer number: 16
I (1117) wifi:Init max length of beacon: 752/752
I (1117) wifi:Init max length of beacon: 752/752
I (1117) wifi_ap: WiFi SoftAP started. SSID: ESP32-TEST, Password: niwenwoa
I (1117) esp_netif_lwip: DHCP server started on interface WIFI_AP_DEF with IP: 192.168.4.1
I (1137) wifi_ap: DHCP server is running.
I (1137) PSRAM: PSRAM size: 8388608 bytes
I (1137) PSRAM: Free heap: 8660424 bytes
I (1147) PSRAM: PSRAM is initialized and ready.
I (1147) PSRAM: Successfully allocated 1KB from PSRAM.
I (1157) camera: : Initializing camera sensor...
W (1197) camera: : VSYNC signal not detected
I (1197) camera: : Camera sensor initialization complete.
I (1287) SPIFFS:: SPIFFS mounted successfully
I (1287) webserver: Starting webserver...
I (1287) webserver: Webserver started
系统初始化完成，等待客户端连接...
I (1287) main_task: Returned from app_main()
I (6157) wifi:new:<1,1>, old:<1,1>, ap:<1,1>, sta:<255,255>, prof:1, snd_ch_cfg:0x0
I (6157) wifi:station: 5c:5f:67:ed:16:1a join, AID=1, bgn, 40U
I (6187) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (9497) webserver: HTTP POST /toggle
I (9507) webserver: Received toggle command: ON
I (9567) wifi:<ba-add>idx:2 (ifx:1, 5c:5f:67:ed:16:1a), tid:0, ssn:53, winSize:64
I (11597) webserver: HTTP GET /image
I (11597) webserver: Allocating frame buffer (153600 bytes)
I (11597) webserver: Free heap before alloc: 8650364
I (11607) OV7670_HANDLER: Reading raw frame of size: 153600
I (11607) OV7670_HANDLER: Resetting FIFO read pointer...
I (11617) OV7670_HANDLER: Enabling FIFO output...
I (11617) OV7670_HANDLER: Byte[0] = 0x00
I (11617) OV7670_HANDLER: Byte[1] = 0x00
I (11627) OV7670_HANDLER: Byte[2] = 0x0C
I (11627) OV7670_HANDLER: Byte[3] = 0xEC
I (12217) OV7670_HANDLER: Disabling FIFO output...
I (12217) OV7670_HANDLER: Raw frame read complete
I (12217) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (12217) jpeg: Allocating RGB888 buffer: 230400 bytes
I (12227) jpeg: Converting RGB565 to RGB888...
I (12227) jpeg: First pixel RGB565: 0x0000 → RGB888: R=0 G=0 B=0
I (12247) jpeg: Starting JPEG encoding...
W (12257) jpeg: JPEG buffer expanded to 307200 bytes
I (12267) jpeg: JPEG encoding successful: 230422 bytes
I (12267) webserver: JPEG encoded size: 230422 bytes


系统初始化完成，等待客户端连接...
I (1186) main_task: Returned from app_main()
I (6066) wifi:new:<1,1>, old:<1,1>, ap:<1,1>, sta:<255,255>, prof:1, snd_ch_cfg:0x0
I (6066) wifi:station: 5c:5f:67:ed:16:1a join, AID=1, bgn, 40U
I (6096) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (9696) wifi:<ba-add>idx:2 (ifx:1, 5c:5f:67:ed:16:1a), tid:0, ssn:55, winSize:64
I (12896) webserver: HTTP POST /toggle
I (12896) webserver: Received toggle command: ON
I (15876) webserver: HTTP GET /image
I (15876) webserver: Allocating frame buffer (153600 bytes)
I (15876) webserver: Free heap before alloc: 8650740
I (15876) OV7670_HANDLER: Reading raw frame of size: 153600
I (15886) OV7670_HANDLER: Resetting FIFO read pointer...
I (15886) OV7670_HANDLER: Enabling FIFO output...
I (15896) OV7670_HANDLER: Byte[0] = 0xE8
I (15896) OV7670_HANDLER: Byte[1] = 0x40
I (15906) OV7670_HANDLER: Byte[2] = 0x00
I (15906) OV7670_HANDLER: Byte[3] = 0x00
I (16486) OV7670_HANDLER: Disabling FIFO output...
I (16486) OV7670_HANDLER: Raw frame read complete
I (16496) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (16496) jpeg: Allocating RGB888 buffer: 230400 bytes
I (16496) jpeg: Converting RGB565 to RGB888...
I (16506) jpeg: First pixel RGB565: 0x40E8 → RGB888: R=64 G=28 B=64
I (16526) jpeg: Starting JPEG encoding...
W (16526) jpeg: JPEG buffer expanded to 307200 bytes
I (16546) jpeg: JPEG encoding successful: 230422 bytes
I (16546) webserver: JPEG encoded size: 230422 bytes
I (26636) webserver: HTTP GET /image
I (26636) webserver: Allocating frame buffer (153600 bytes)
I (26636) webserver: Free heap before alloc: 8650740
I (26636) OV7670_HANDLER: Reading raw frame of size: 153600
I (26636) OV7670_HANDLER: Resetting FIFO read pointer...
I (26646) OV7670_HANDLER: Enabling FIFO output...
I (26646) OV7670_HANDLER: Byte[0] = 0xE8
I (26656) OV7670_HANDLER: Byte[1] = 0xE8
I (26656) OV7670_HANDLER: Byte[2] = 0x00
I (26656) OV7670_HANDLER: Byte[3] = 0x00
I (27246) OV7670_HANDLER: Disabling FIFO output...
I (27246) OV7670_HANDLER: Raw frame read complete
I (27246) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (27246) jpeg: Allocating RGB888 buffer: 230400 bytes
I (27256) jpeg: Converting RGB565 to RGB888...
I (27256) jpeg: First pixel RGB565: 0xE8E8 → RGB888: R=232 G=28 B=64
I (27276) jpeg: Starting JPEG encoding...
W (27286) jpeg: JPEG buffer expanded to 307200 bytes
I (27296) jpeg: JPEG encoding successful: 230422 bytes
I (27296) webserver: JPEG encoded size: 230422 bytes
I (33706) webserver: HTTP GET /image
I (33706) webserver: Allocating frame buffer (153600 bytes)
I (33706) webserver: Free heap before alloc: 8650740
I (33716) OV7670_HANDLER: Reading raw frame of size: 153600
I (33716) OV7670_HANDLER: Resetting FIFO read pointer...
I (33726) OV7670_HANDLER: Enabling FIFO output...
I (33726) OV7670_HANDLER: Byte[0] = 0xE8
I (33726) OV7670_HANDLER: Byte[1] = 0xE8
I (33736) OV7670_HANDLER: Byte[2] = 0xE8
I (33736) OV7670_HANDLER: Byte[3] = 0x00
I (34326) OV7670_HANDLER: Disabling FIFO output...
I (34326) OV7670_HANDLER: Raw frame read complete
I (34326) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (34326) jpeg: Allocating RGB888 buffer: 230400 bytes
I (34336) jpeg: Converting RGB565 to RGB888...
I (34336) jpeg: First pixel RGB565: 0xE8E8 → RGB888: R=232 G=28 B=64
I (34356) jpeg: Starting JPEG encoding...
W (34366) jpeg: JPEG buffer expanded to 307200 bytes
I (34376) jpeg: JPEG encoding successful: 230422 bytes
I (34376) webserver: JPEG encoded size: 230422 bytes
I (37976) webserver: HTTP GET /image
I (37976) webserver: Allocating frame buffer (153600 bytes)
I (37976) webserver: Free heap before alloc: 8650740
I (37976) OV7670_HANDLER: Reading raw frame of size: 153600
I (37986) OV7670_HANDLER: Resetting FIFO read pointer...
I (37986) OV7670_HANDLER: Enabling FIFO output...
I (37996) OV7670_HANDLER: Byte[0] = 0xE8
I (37996) OV7670_HANDLER: Byte[1] = 0x00
I (37996) OV7670_HANDLER: Byte[2] = 0x00
I (38006) OV7670_HANDLER: Byte[3] = 0x00
I (38596) OV7670_HANDLER: Disabling FIFO output...
I (38596) OV7670_HANDLER: Raw frame read complete
I (38596) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (38596) jpeg: Allocating RGB888 buffer: 230400 bytes
I (38596) jpeg: Converting RGB565 to RGB888...
I (38606) jpeg: First pixel RGB565: 0x00E8 → RGB888: R=0 G=28 B=64
I (38626) jpeg: Starting JPEG encoding...
W (38626) jpeg: JPEG buffer expanded to 307200 bytes
I (38646) jpeg: JPEG encoding successful: 230422 bytes
I (38646) webserver: JPEG encoded size: 230422 bytes




I (24696) webserver: HTTP GET /image
I (24696) webserver: Allocating frame buffer (153600 bytes)
I (24696) webserver: Free heap before alloc: 8650752
I (24696) OV7670_HANDLER: Reading raw frame of size: 153600
I (24706) OV7670_HANDLER: Resetting FIFO read pointer...
I (24706) OV7670_HANDLER: Enabling FIFO output...
I (24716) OV7670_HANDLER: Byte[0] = 0x00
I (24716) OV7670_HANDLER: Byte[1] = 0xE8
I (24716) OV7670_HANDLER: Byte[2] = 0xE8
I (24726) OV7670_HANDLER: Byte[3] = 0x60
I (25306) OV7670_HANDLER: Disabling FIFO output...
I (25306) OV7670_HANDLER: Raw frame read complete
I (25316) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (25316) jpeg: Allocating RGB888 buffer: 230400 bytes
I (25316) jpeg: Converting RGB565 to RGB888...
I (25326) jpeg: First pixel RGB565: 0xE800 → RGB888: R=232 G=0 B=0
I (25346) jpeg: Starting JPEG encoding...
W (25346) jpeg: JPEG buffer expanded to 307200 bytes
I (25356) jpeg: JPEG encoding successful: 230422 bytes
I (25356) webserver: JPEG encoded size: 230422 bytes


2025-9-28:
I (24556) webserver: HTTP GET /image
I (24556) webserver: Allocating frame buffer (153600 bytes)
I (24556) webserver: Free heap before alloc: 8650740
I (25146) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (25146) jpeg: Allocating RGB888 buffer: 230400 bytes
I (25146) jpeg: Converting RGB565 to RGB888...
I (25146) jpeg: First pixel RGB565: 0x0000 → RGB888: R=0 G=0 B=0
I (25166) jpeg: Starting JPEG encoding...
W (25176) jpeg: JPEG buffer expanded to 307200 bytes
I (25186) jpeg: JPEG encoding successful: 230422 bytes
I (25186) webserver: JPEG encoded size: 230422 bytes


I (12166) webserver: HTTP GET /
I (12176) webserver: HTTP POST /toggle
I (12176) webserver: Received toggle command: ON
I (13086) webserver: HTTP GET /favicon.ico
I (13776) webserver: HTTP GET /image
I (13776) webserver: Allocating frame buffer (153600 bytes)
I (13776) webserver: Free heap before alloc: 8650728
I (14356) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (14356) jpeg: Allocating RGB888 buffer: 230400 bytes
E (14356) jpeg: Failed to allocate RGB888 buffer
E (14366) webserver: JPEG encoding failed
W (14366) httpd_txrx: httpd_resp_send_err: 500 Internal Server Error - Server has encountered an unexpected error
W (14376) httpd_uri: httpd_uri: uri handler execution failed

I (11696) webserver: HTTP GET /
I (11806) webserver: HTTP GET /favicon.ico
I (14696) webserver: HTTP POST /toggle
I (14696) webserver: Received toggle command: ON
I (16096) webserver: HTTP GET /image
I (16096) webserver: Allocating frame buffer (153600 bytes)
I (16096) webserver: Free heap before alloc: 8650412
I (16676) jpeg: Allocating initial JPEG buffer: 76800 bytes
I (16676) jpeg: Allocating RGB888 buffer: 230400 bytes
I (16676) jpeg: Converting RGB565 to RGB888...
I (16686) jpeg: First pixel RGB565: 0x0000 → RGB888: R=0 G=0 B=0
I (16706) jpeg: Starting JPEG encoding...
E (16706) jpeg: Failed to realloc JPEG buffer
I (16706) jpeg: JPEG encoding successful: 22 bytes
I (16706) webserver: JPEG encoded size: 22 bytes



I (17956) jpeg: Allocating JPEG buffer: 76800 bytes
I (17956) jpeg: Allocating RGB888 buffer: 230400 bytes
I (17956) jpeg: Converting RGB565 to RGB888...
I (17956) jpeg: First pixel RGB565: 0xE8E8 → RGB888: R=232 G=28 B=64
I (17976) jpeg: Starting JPEG encoding...
E (17976) jpeg: JPEG buffer overflow: size=230420, capacity=76800
I (17976) jpeg: JPEG encoding successful: 22 bytes
I (17986) webserver: JPEG encoded size: 22 bytes


2025-09-27
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:41:7: note: in expansion of macro 'REG_COM7'
   41 |     { REG_COM7, 0x80 },     //reverse
      |       ^~~~~~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera_reg.h:31:25: note: (near initialization for 'ov7670_qvga_rgb565')
   31 | #define REG_COM7        0x12
      |                         ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:41:7: note: in expansion of macro 'REG_COM7'
   41 |     { REG_COM7, 0x80 },     //reverse
      |       ^~~~~~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:41:17: warning: excess elements in scalar initializer
   41 |     { REG_COM7, 0x80 },     //reverse
      |                 ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:41:17: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:42:5: warning: braces around scalar initializer
   42 |     { REG_CLKRC, 0x80 },    //use external clk signal
      |     ^
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:42:5: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera_reg.h:30:25: error: initialization of 'const struct regval_list *' from 'int' makes pointer from integer without a cast [-Wint-conversion]
   30 | #define REG_CLKRC       0x11
      |                         ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:42:7: note: in expansion of macro 'REG_CLKRC'
   42 |     { REG_CLKRC, 0x80 },    //use external clk signal
      |       ^~~~~~~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera_reg.h:30:25: note: (near initialization for 'ov7670_qvga_rgb565')
   30 | #define REG_CLKRC       0x11
      |                         ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:42:7: note: in expansion of macro 'REG_CLKRC'
   42 |     { REG_CLKRC, 0x80 },    //use external clk signal
      |       ^~~~~~~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:42:18: warning: excess elements in scalar initializer
   42 |     { REG_CLKRC, 0x80 },    //use external clk signal
      |                  ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:42:18: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:42:5: warning: excess elements in scalar initializer
   42 |     { REG_CLKRC, 0x80 },    //use external clk signal
      |     ^
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:42:5: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:43:5: warning: braces around scalar initializer
   43 |     { REG_COM11, 0x0A },    //use BD50ST and expourse time smaller
      |     ^
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:43:5: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera_reg.h:63:25: error: initialization of 'const struct regval_list *' from 'int' makes pointer from integer without a cast [-Wint-conversion]
   63 | #define REG_COM11       0x3B
      |                         ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:43:7: note: in expansion of macro 'REG_COM11'
   43 |     { REG_COM11, 0x0A },    //use BD50ST and expourse time smaller
      |       ^~~~~~~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera_reg.h:63:25: note: (near initialization for 'ov7670_qvga_rgb565')
   63 | #define REG_COM11       0x3B
      |                         ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:43:7: note: in expansion of macro 'REG_COM11'
   43 |     { REG_COM11, 0x0A },    //use BD50ST and expourse time smaller
      |       ^~~~~~~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:43:18: warning: excess elements in scalar initializer
   43 |     { REG_COM11, 0x0A },    //use BD50ST and expourse time smaller
      |                  ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:43:18: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:43:5: warning: excess elements in scalar initializer
   43 |     { REG_COM11, 0x0A },    //use BD50ST and expourse time smaller
      |     ^
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:43:5: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:44:5: warning: braces around scalar initializer
   44 |     { REG_COM7, 0x04 },     //output RGB mode
      |     ^
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:44:5: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera_reg.h:31:25: error: initialization of 'const struct regval_list *' from 'int' makes pointer from integer without a cast [-Wint-conversion]
   31 | #define REG_COM7        0x12
      |                         ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:44:7: note: in expansion of macro 'REG_COM7'
   44 |     { REG_COM7, 0x04 },     //output RGB mode
      |       ^~~~~~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera_reg.h:31:25: note: (near initialization for 'ov7670_qvga_rgb565')
   31 | #define REG_COM7        0x12
      |                         ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:44:7: note: in expansion of macro 'REG_COM7'
   44 |     { REG_COM7, 0x04 },     //output RGB mode
      |       ^~~~~~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:44:17: warning: excess elements in scalar initializer
   44 |     { REG_COM7, 0x04 },     //output RGB mode
      |                 ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:44:17: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:44:5: warning: excess elements in scalar initializer
   44 |     { REG_COM7, 0x04 },     //output RGB mode
      |     ^
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:44:5: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:45:5: warning: braces around scalar initializer
   45 |     { REG_RGB444, 0x00 },    //RGB44 disable
      |     ^
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:45:5: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera_reg.h:127:25: error: initialization of 'const struct regval_list *' from 'int' makes pointer from integer without a cast [-Wint-conversion]
  127 | #define REG_RGB444      0x8C
      |                         ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:45:7: note: in expansion of macro 'REG_RGB444'
   45 |     { REG_RGB444, 0x00 },    //RGB44 disable
      |       ^~~~~~~~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera_reg.h:127:25: note: (near initialization for 'ov7670_qvga_rgb565')
  127 | #define REG_RGB444      0x8C
      |                         ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:45:7: note: in expansion of macro 'REG_RGB444'
   45 |     { REG_RGB444, 0x00 },    //RGB44 disable
      |       ^~~~~~~~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:45:19: warning: excess elements in scalar initializer
   45 |     { REG_RGB444, 0x00 },    //RGB44 disable
      |                   ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:45:19: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:45:5: warning: excess elements in scalar initializer
   45 |     { REG_RGB444, 0x00 },    //RGB44 disable
      |     ^
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:45:5: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:46:5: warning: braces around scalar initializer
   46 |     { REG_COM15, 0xD0 },    //output 00-FF, RGB565 match with RGB444 low
      |     ^
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:46:5: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera_reg.h:68:25: error: initialization of 'const struct regval_list *' from 'int' makes pointer from integer without a cast [-Wint-conversion]
   68 | #define REG_COM15       0x40
      |                         ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:46:7: note: in expansion of macro 'REG_COM15'
   46 |     { REG_COM15, 0xD0 },    //output 00-FF, RGB565 match with RGB444 low
      |       ^~~~~~~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera_reg.h:68:25: note: (near initialization for 'ov7670_qvga_rgb565')
   68 | #define REG_COM15       0x40
      |                         ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:46:7: note: in expansion of macro 'REG_COM15'
   46 |     { REG_COM15, 0xD0 },    //output 00-FF, RGB565 match with RGB444 low
      |       ^~~~~~~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:46:18: warning: excess elements in scalar initializer
   46 |     { REG_COM15, 0xD0 },    //output 00-FF, RGB565 match with RGB444 low
      |                  ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:46:18: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:46:5: warning: excess elements in scalar initializer
   46 |     { REG_COM15, 0xD0 },    //output 00-FF, RGB565 match with RGB444 low
      |     ^
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:46:5: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:47:5: warning: braces around scalar initializer
   47 |     { REG_COM1, 0x00 },    ///disable general control
      |     ^
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:47:5: note: (near initialization for 'ov7670_qvga_rgb565')
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera_reg.h:17:25: error: initialization of 'const struct regval_list *' from 'int' makes pointer from integer without a cast [-Wint-conversion]
   17 | #define REG_COM1        0x04
      |                         ^~~~
/Users/maochun/esp32prj/Motor/motor_wifi/esp32_wifi_control/components/camera/camera.c:47:7: note: in expansion of macro 'REG_COM1'
   47 |     { REG_COM1, 0x00 },    ///disable general control
      |       ^~~~~~~~