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





Mac PC addr
I (38757) wifi:new:<1,0>, old:<1,1>, ap:<1,0>, sta:<255,255>, prof:1, snd_ch_cfg:0x0
I (38757) wifi:station: a4:5e:60:c5:69:75 join, AID=1, bgn, 20
I (38777) wifi_ap: Client connected: MAC=A4:5E:60:C5:69:75, AID=1
I (38777) wifi_ap: Normal client, DHCP will assign IP >= 192.168.4.3
I (38867) wifi:<ba-add>idx:2 (ifx:1, a4:5e:60:c5:69:75), tid:6, ssn:2, winSize:64
I (39887) esp_netif_lwip: DHCP server assigned IP to a client, IP is: 192.168.4.2
I (41777) wifi:<ba-add>idx:3 (ifx:1, a4:5e:60:c5:69:75), tid:0, ssn:0, winSize:64
I (48277) wifi:<ba-del>idx:3, tid:0
I (48287) wifi:<ba-add>idx:3 (ifx:1, a4:5e:60:c5:69:75), tid:0, ssn:62, winSize:64







