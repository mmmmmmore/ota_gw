#include "wifi_ap.h"
#include "sdkconfig.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
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
                 event->mac[3], event->mac[4], event->mac[5],
                 event->aid);
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI("wifi_ap", "Client disconnected: MAC=%02X:%02X:%02X:%02X:%02X:%02X, AID=%d",
                 event->mac[0], event->mac[1], event->mac[2],
                 event->mac[3], event->mac[4], event->mac[5],
                 event->aid);
    }
}





esp_err_t wifi_init_softap(void)
{
    esp_err_t err;

    // 初始化网络栈
    err = esp_netif_init();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_netif_init failed : %s", esp_err_to_name(err));
        return;
    }

    err = esp_event_loop_create_default();
    if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
        ESP_LOGE(TAG, "esp_event_loop_create_default failed: %s", esp_err_to_name(err));
        return;
    }

    // 创建默认的 WiFi AP 接口
    esp_netif_t *netif = esp_netif_create_default_wifi_ap();

    // 初始化 WiFi 驱动
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 注册事件回调
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    
    // 配置 SoftAP 参数
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .password = WIFI_PASS,
            .max_connection = 6,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };

    if (strlen(WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    err = esp_wifi_set_mode(WIFI_MODE_AP);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_set_mode failed: %s", esp_err_to_name(err));
        return;
    }

    err = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_set_config failed: %s", esp_err_to_name(err));
        return;
    }

    err = esp_wifi_start();
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_wifi_start failed: %s", esp_err_to_name(err));
        return;
    }

    ESP_LOGI(TAG, "WiFi SoftAP started. SSID: %s, Password: %s", WIFI_SSID, WIFI_PASS);

    // 配置 DHCP Server 固定 IP
    esp_netif_ip_info_t ip_info;
    IP4_ADDR(&ip_info.ip, 192, 168, 4, 1);   // GW 自身 IP
    IP4_ADDR(&ip_info.gw, 192, 168, 4, 1);
    IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0);
    esp_netif_set_ip_info(netif, &ip_info);

    // 停止 DHCP Server，重新配置
    esp_netif_dhcps_stop(netif);

    // OTA Server 固定 IP
    esp_netif_dhcps_offer_option(netif, ESP_NETIF_OPT_MAC, ota_server_mac, sizeof(ota_server_mac));
    esp_ip4_addr_t ota_server_ip;
    IP4_ADDR(&ota_server_ip, 192, 168, 4, 2);
    esp_netif_dhcps_offer_option(netif, ESP_NETIF_OPT_IP, &ota_server_ip, sizeof(ota_server_ip));

    // 启动 DHCP Server
    esp_netif_dhcps_start(netif);

    ESP_LOGI(TAG, "DHCP server started with OTA Server fixed IP 192.168.4.2");
    ESP_LOGI(TAG, "OTA Server MAC: %02X:%02X:%02X:%02X:%02X:%02X",
         ota_server_mac[0], ota_server_mac[1], ota_server_mac[2],
         ota_server_mac[3], ota_server_mac[4], ota_server_mac[5]);

    return ESP_OK;
}

