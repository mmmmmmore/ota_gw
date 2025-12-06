#include "wifi_ap.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "lwip/ip_addr.h"
#include <string.h>

static const char *TAG = "wifi_ap";

#define WIFI_SSID CONFIG_WIFI_SOFTAP_SSID
#define WIFI_PASS CONFIG_WIFI_SOFTAP_PASSWORD

esp_err_t wifi_init_softap(void)
{
    // 初始化网络接口和事件循环
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // 创建默认的 AP 接口
    esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();

    // 初始化 WiFi 驱动
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 配置 AP 参数
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .channel = 1,
            .password = WIFI_PASS,
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            .pmf_cfg = {
                .capable = false,   // disable  PMF
                .required = false  // 不强制 PMF，避免 SA Query 误断连
            }
        },
    };

    if (strlen((char *)wifi_config.ap.password) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    // 设置为 AP 模式并应用配置
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));

    // 设置 AP 支持的协议 (11b/g/n)
    ESP_ERROR_CHECK(esp_wifi_set_protocol(WIFI_IF_AP,
        WIFI_PROTOCOL_11B | WIFI_PROTOCOL_11G | WIFI_PROTOCOL_11N));

    // 启动 Wi-Fi
    ESP_ERROR_CHECK(esp_wifi_start());

    // 设置 AP 的网关 IP 信息（必须先停 DHCP）
    ESP_ERROR_CHECK(esp_netif_dhcps_stop(ap_netif));

    esp_netif_ip_info_t ip_info;
    IP4_ADDR(&ip_info.ip, 192,168,4,1);
    IP4_ADDR(&ip_info.gw, 192,168,4,1);
    IP4_ADDR(&ip_info.netmask, 255,255,255,0);
    ESP_ERROR_CHECK(esp_netif_set_ip_info(ap_netif, &ip_info));

    // 重新启动 DHCP server
    ESP_ERROR_CHECK(esp_netif_dhcps_start(ap_netif));

    ESP_LOGI(TAG,
             "SoftAP started: SSID=%s, gateway=192.168.4.1, netmask=255.255.255.0, protocol=b/g/n, PMF=capable/not-required",
             WIFI_SSID);

    return ESP_OK;
}
