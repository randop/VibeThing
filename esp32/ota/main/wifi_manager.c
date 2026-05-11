#include "wifi_manager.h"
#include "app_config.h"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp_log.h"

static const char *TAG = "wifi_mgr";

static EventGroupHandle_t    s_event_group;
static esp_netif_t          *s_sta_netif;
static esp_event_handler_instance_t s_wifi_handler;
static esp_event_handler_instance_t s_ip_handler;
static int                   s_retry;

static void on_wifi_event(void *arg, esp_event_base_t base,
                           int32_t id, void *data)
{
    (void)arg;
    (void)data;
    (void)base;

    switch (id) {
    case WIFI_EVENT_STA_START:
        esp_wifi_connect();
        break;

    case WIFI_EVENT_STA_DISCONNECTED:
        if (s_retry < CONFIG_OTA_WIFI_MAX_RETRY) {
            s_retry++;
            ESP_LOGW(TAG, "Reconnect attempt %d/%d",
                     s_retry, CONFIG_OTA_WIFI_MAX_RETRY);
            esp_wifi_connect();
        } else {
            xEventGroupSetBits(s_event_group, WIFI_FAILED_BIT);
        }
        break;

    default:
        break;
    }
}

static void on_ip_event(void *arg, esp_event_base_t base,
                         int32_t id, void *data)
{
    (void)arg;
    (void)base;

    if (id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t *event = data;
        ESP_LOGI(TAG, "IP: " IPSTR, IP2STR(&event->ip_info.ip));
        s_retry = 0;
        xEventGroupSetBits(s_event_group, WIFI_CONNECTED_BIT);
    }
}

esp_err_t wifi_manager_init(void)
{
    s_event_group = xEventGroupCreate();
    if (!s_event_group) {
        return ESP_ERR_NO_MEM;
    }

    s_sta_netif = esp_netif_create_default_wifi_sta();
    if (!s_sta_netif) {
        vEventGroupDelete(s_event_group);
        return ESP_FAIL;
    }

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_err_t ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK) {
        goto err_netif;
    }

    ret = esp_event_handler_instance_register(
            WIFI_EVENT, ESP_EVENT_ANY_ID,
            on_wifi_event, NULL, &s_wifi_handler);
    if (ret != ESP_OK) goto err_wifi;

    ret = esp_event_handler_instance_register(
            IP_EVENT, IP_EVENT_STA_GOT_IP,
            on_ip_event, NULL, &s_ip_handler);
    if (ret != ESP_OK) goto err_wifi_handler;

    ret = esp_wifi_set_mode(WIFI_MODE_STA);
    if (ret != ESP_OK) goto err_ip_handler;

    return ESP_OK;

err_ip_handler:
    esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP,
                                           s_ip_handler);
err_wifi_handler:
    esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                           s_wifi_handler);
err_wifi:
    esp_wifi_deinit();
err_netif:
    esp_netif_destroy(s_sta_netif);
    s_sta_netif = NULL;
    vEventGroupDelete(s_event_group);
    return ret;
}

esp_err_t wifi_manager_connect(void)
{
    wifi_config_t cfg = {
        .sta = {
            .ssid     = CONFIG_OTA_WIFI_SSID,
            .password = CONFIG_OTA_WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable  = true,
                .required = false,
            },
        },
    };

    esp_err_t ret = esp_wifi_set_config(WIFI_IF_STA, &cfg);
    if (ret != ESP_OK) {
        return ret;
    }

    s_retry = 0;
    xEventGroupClearBits(s_event_group,
                         WIFI_CONNECTED_BIT | WIFI_FAILED_BIT);

    ret = esp_wifi_start();
    if (ret != ESP_OK) {
        return ret;
    }

    EventBits_t bits = xEventGroupWaitBits(
            s_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAILED_BIT,
            pdFALSE, pdFALSE,
            portMAX_DELAY);

    if (bits & WIFI_CONNECTED_BIT) {
        return ESP_OK;
    }

    ESP_LOGE(TAG, "Failed to connect after %d attempts",
             CONFIG_OTA_WIFI_MAX_RETRY);
    return ESP_FAIL;
}

void wifi_manager_disconnect(void)
{
    esp_wifi_disconnect();
    esp_wifi_stop();
}
