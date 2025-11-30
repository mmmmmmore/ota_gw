Protocol for wifi, network, https relate function low level realization

在 wifi_init_softap() 里，你的流程是：

初始化网络栈 → esp_netif_init()
创建 AP 接口 → esp_netif_create_default_wifi_ap()
初始化 WiFi 驱动 → esp_wifi_init()
配置 AP 参数 → esp_wifi_set_config()
启动 WiFi → esp_wifi_start()
配置 DHCP Server 固定 IP → esp_netif_set_ip_info()
停止 DHCP → esp_netif_dhcps_stop()
设置 OTA Server 的固定 IP → esp_netif_dhcps_offer_option()
启动 DHCP → esp_netif_dhcps_start()













