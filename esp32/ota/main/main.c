#include "app_config.h"
#include "wifi_manager.h"
#include "ota_manager.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_ota_ops.h"

static const char *TAG = "VibeThing";

static void ota_task(void *arg)
{
    (void)arg;

    esp_err_t ret = wifi_manager_connect();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Wi-Fi connect failed: %s", esp_err_to_name(ret));
        goto done;
    }

    ret = ota_manager_run();

    switch (ret) {
    case ESP_ERR_NOT_FOUND:
        ESP_LOGI(TAG, "Firmware is up to date");
        break;
    case ESP_ERR_OTA_VALIDATE_FAILED:
        ESP_LOGE(TAG, "Remote image rejected (version gate or name mismatch)");
        break;
    default:
        ESP_LOGE(TAG, "OTA failed: %s", esp_err_to_name(ret));
        break;
    }

    wifi_manager_disconnect();

done:
    vTaskDelete(NULL);
}

/*
Application self-test hook:
Replace the body with real hardware / connectivity checks.
Return false to trigger immediate rollback.
*/
static bool app_self_test(void)
{
    return true;
}

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    const esp_app_desc_t *desc = esp_app_get_description();
    ESP_LOGI(TAG, "Booting  project=%s  version=%s",
             desc->project_name, desc->version);

    if (app_self_test()) {
        ret = ota_manager_confirm_running_image();
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to confirm OTA image: %s",
                     esp_err_to_name(ret));
        }
    } else {
        ESP_LOGE(TAG, "Self-test failed — triggering rollback");
        esp_ota_mark_app_invalid_rollback_and_reboot();
    }

    ESP_ERROR_CHECK(wifi_manager_init());

    BaseType_t task_ret = xTaskCreate(
            ota_task, "ota",
            CONFIG_OTA_TASK_STACK_SIZE,
            NULL,
            CONFIG_OTA_TASK_PRIORITY,
            NULL);

    if (task_ret != pdPASS) {
        ESP_LOGE(TAG, "Failed to create OTA task");
    }

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}
