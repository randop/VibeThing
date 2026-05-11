#pragma once

#include "esp_err.h"

/**
 * Initialise the Wi-Fi driver and create the default STA netif.
 * Must be called once after esp_netif_init() and
 * esp_event_loop_create_default().
 */
esp_err_t wifi_manager_init(void);

/**
 * Connect to the AP configured via Kconfig (OTA_WIFI_SSID / PASSWORD).
 * Blocks until an IP address is obtained or the retry limit is reached.
 *
 * @return ESP_OK on success, ESP_FAIL on auth/network error.
 */
esp_err_t wifi_manager_connect(void);

/**
 * Disconnect from AP and stop the Wi-Fi driver.
 */
void wifi_manager_disconnect(void);
