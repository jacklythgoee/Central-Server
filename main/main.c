#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_netif.h"
#include <string.h>

// Function to initialize the Wi-Fi as an AP
void wifi_init_softap() {
    // Create a default network interface for AP mode
    esp_netif_create_default_wifi_ap();
    
    // Default WiFi configuration parameters
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Configuration settings for the AP
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "ESP32-AP",
            .ssid_len = strlen("ESP32-AP"),
            .channel = 1,
            .password = "your_password",
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };

    // If password length is 0, set the AP to open (no security)
    if (strlen("your_password") == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    // Set the Wi-Fi mode to AP only
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    // Apply the AP configuration settings
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    // Start the Wi-Fi service
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI("wifi_init_softap", "Wi-Fi set up as AP with SSID: %s", wifi_config.ap.ssid);
}

// Main application entry point
void app_main() {
    // Initialize NVS — required for Wi-Fi
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initialize the TCP/IP stack
    ESP_ERROR_CHECK(esp_netif_init());
    // Create the default event loop
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize and start Wi-Fi as AP
    wifi_init_softap();
}
