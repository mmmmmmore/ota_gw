#include "wifi_ap.h"
#include "sdkconfig.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "lwip/ip4_addr.h"
#include <string.h>

static const char *TAG = "wifi_ap";

// 从 sdkconfig 中读取 SSID 和密码
#define WIFI_SSID CONFIG_WIFI_SOFTAP_SSID
#define WIFI_PASS CONFIG_WIFI_SOFTAP_PASSWORD

// OTA Server 的 MAC 地址（示例，需替换为真实值）
static const uint8_t ota_server_mac[6] = {0xAA,0xBB,0xCC,0xDD,0xEE,0xFF};


static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI("wifi_ap", "Client connected: MAC=%02X:%02X:%02X:%02X:%02X:%02X, AID=%d",
                 event->mac[0], event->mac[1], event->mac[2],
                 event->mac[3], event->mac[4], event->mac[5], event->aid);

        // 判断是否为 OTA Server
        if (memcmp(event->mac, ota_server_mac, 6) == 0) {
            ESP_LOGI("wifi_ap", "OTA Server detected, assigning fixed IP 192.168.4.2");
            // TODO: 调用 DHCP API 设置静态租约
        } else {
            ESP_LOGI("wifi_ap", "Normal client, DHCP will assign IP >= 192.168.4.3");
        }

    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI("wifi_ap", "Client disconnected: MAC=%02X:%02X:%02X:%02X:%02X:%02X, AID=%d",
                 event->mac[0], event->mac[1], event->mac[2],
                 event->mac[3], event->mac[4], event->mac[5], event->aid);
    }
}



esp_err_t wifi_init_softap(void)
{
    // 初始化网络接口和事件循环
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // 创建默认的 AP 接口
    esp_netif_t *netif = esp_netif_create_default_wifi_ap();

    // 初始化 WiFi 驱动
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 配置 AP 参数
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .channel = 1,
            .password = "WIFI_PASS",
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


    ESP_ERROR_CHECK( esp_event_handler_instance_register(WIFI_EVENT,
                                                     WIFI_EVENT_AP_STACONNECTED,
                                                     &wifi_event_handler,
                                                     NULL,
                                                     NULL));

    ESP_ERROR_CHECK( esp_event_handler_instance_register(WIFI_EVENT,
                                                     WIFI_EVENT_AP_STADISCONNECTED,
                                                     &wifi_event_handler,
                                                     NULL,
                                                     NULL));

    ESP_LOGI(TAG, "WiFi SoftAP started. SSID:%s password:%s channel:%d",
             wifi_config.ap.ssid, wifi_config.ap.password, wifi_config.ap.channel);

    // 设置网关 IP 信息
    esp_netif_ip_info_t ip_info;
    IP4_ADDR(&ip_info.ip, 192, 168, 4, 1);
    IP4_ADDR(&ip_info.gw, 192, 168, 4, 1);
    IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0);
    ESP_ERROR_CHECK(esp_netif_set_ip_info(netif, &ip_info));

    // 停止并重新启动 DHCP 服务，应用新的 IP 配置
    ESP_ERROR_CHECK(esp_netif_dhcps_stop(netif));
    ESP_ERROR_CHECK(esp_netif_dhcps_start(netif));

    return ESP_OK;
}


