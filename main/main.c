#include "esp_event.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include <stdio.h>
#include <string.h>

#define MAXIMUM_AP_SCAN 20
static void  wifi_init(void);
static void  wifi_scan(void);
static char* getAuthModeName(wifi_auth_mode_t auth_mode);

void app_main(void)
{
    wifi_init();
    wifi_scan();

    return;
}

static void wifi_init(void)
{
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_config);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
}

static void wifi_scan(void)
{
    wifi_scan_config_t wifi_scan_config;
    memset(&wifi_scan_config, 0, sizeof(wifi_scan_config));
    wifi_scan_config.show_hidden = true;

    esp_wifi_scan_start(&wifi_scan_config, true);
    wifi_ap_record_t wifi_ap_records[MAXIMUM_AP_SCAN];

    uint16_t max_number_of_ap_found = MAXIMUM_AP_SCAN;
    esp_wifi_scan_get_ap_records(&max_number_of_ap_found, wifi_ap_records);

    printf("Found %d access points:\n", max_number_of_ap_found);
    printf("\n");
    printf("               SSID              | Channel | RSSI |   Auth Mode \n");
    printf("----------------------------------------------------------------\n");
    for (int i = 0; i < max_number_of_ap_found; i++)
        printf("%32s | %7d | %4d | %12s\n", (char*)wifi_ap_records[i].ssid, wifi_ap_records[i].primary,
               wifi_ap_records[i].rssi, getAuthModeName(wifi_ap_records[i].authmode));
    printf("----------------------------------------------------------------\n");
}

static char* getAuthModeName(wifi_auth_mode_t auth_mode)
{
    char* names[] = {"OPEN", "WEP", "WPA PSK", "WPA2 PSK", "WPA WPA2 PSK", "MAX"};
    return names[auth_mode];
}