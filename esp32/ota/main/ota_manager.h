#pragma once

#include "esp_err.h"

/**
 * Download and apply a firmware update from CONFIG_OTA_SERVER_URL.
 *
 * @return
 *   ESP_OK            — Update written; device will restart.
 *   ESP_ERR_NOT_FOUND — Server version equals or is older than running version;
 *                       no update applied.
 *   ESP_FAIL / other  — Transport, TLS, or image validation error.
 *
 * Never returns on success (triggers esp_restart).
 */
esp_err_t ota_manager_run(void);

/**
 * Call after the application has completed its self-test.
 * Marks the running OTA image as confirmed and cancels the automatic rollback
 * timer.  No-op when booting from factory partition.
 *
 * @return ESP_OK or an esp_ota error code.
 */
esp_err_t ota_manager_confirm_running_image(void);
