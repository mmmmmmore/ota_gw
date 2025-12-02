#include "wifi_ap.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "lwip/ip_addr.h"   // 提供 IP4_ADDR 宏
#include <string.h>

static const char *TAG = "wifi_ap";

#define WIFI_SSID CONFIG_WIFI_SOFTAP_SSID
#define WIFI_PASS CONFIG_WIFI_SOFTAP_PASSWORD

esp_err_t wifi_init_softap(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .channel = 1,
            .password = WIFI_PASS,
            .max_connection = 6,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen((char *)wifi_config.ap.password) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    // 设置 AP 本身的 IP
    esp_netif_ip_info_t ip_info;
    IP4_ADDR(&ip_info.ip, 192,168,4,1);
    IP4_ADDR(&ip_info.gw, 192,168,4,1);
    IP4_ADDR(&ip_info.netmask, 255,255,255,0);
    ESP_ERROR_CHECK(esp_netif_set_ip_info(ap_netif, &ip_info));

    // 停止 DHCP 服务，修改起始地址
    ESP_ERROR_CHECK(esp_netif_dhcps_stop(ap_netif));

    ip4_addr_t start_ip;
    IP4_ADDR(&start_ip, 192,168,4,3);   // 从 192.168.4.3 开始分配
    ESP_ERROR_CHECK(esp_netif_dhcps_option(ap_netif,
                                           ESP_NETIF_OP_SET,
                                           ESP_NETIF_DHCP_START_IP,
                                           &start_ip,
                                           sizeof(start_ip)));

    // 重新启动 DHCP 服务
    ESP_ERROR_CHECK(esp_netif_dhcps_start(ap_netif));

    ESP_LOGI(TAG, "SoftAP started, gateway=192.168.4.1, DHCP pool from 192.168.4.3");

    return ESP_OK;
}
