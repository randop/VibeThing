#include "ota_manager.h"
#include "app_config.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_ota_ops.h"
#include "esp_https_ota.h"
#include "esp_app_desc.h"
#include "esp_http_client.h"

static const char *TAG = "ota_mgr";

extern const uint8_t server_cert_pem_start[] asm("_binary_server_cert_pem_start");
extern const uint8_t server_cert_pem_end[]   asm("_binary_server_cert_pem_end");

typedef struct {
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
} semver_t;

static bool semver_parse(const char *s, semver_t *out)
{
    unsigned int maj = 0, min = 0, pat = 0;
    if (sscanf(s, "%u.%u.%u", &maj, &min, &pat) != 3) {
        return false;
    }
    out->major = (uint16_t)maj;
    out->minor = (uint16_t)min;
    out->patch = (uint16_t)pat;
    return true;
}

/* Returns negative / zero / positive like strcmp. */
static int semver_cmp(const semver_t *a, const semver_t *b)
{
    if (a->major != b->major) return (int)a->major - b->major;
    if (a->minor != b->minor) return (int)a->minor - b->minor;
    return (int)a->patch - b->patch;
}

static esp_err_t validate_image_header(const esp_app_desc_t *incoming)
{
    const esp_app_desc_t *running = esp_app_get_description();

    ESP_LOGI(TAG, "Running  : project=%s version=%s",
             running->project_name, running->version);
    ESP_LOGI(TAG, "Incoming : project=%s version=%s",
             incoming->project_name, incoming->version);

    if (strncmp(incoming->project_name, running->project_name,
                sizeof(incoming->project_name)) != 0) {
        ESP_LOGE(TAG, "Project name mismatch — refusing update");
        return ESP_ERR_OTA_VALIDATE_FAILED;
    }

    semver_t ver_new, ver_cur;
    if (!semver_parse(incoming->version, &ver_new) ||
        !semver_parse(running->version,  &ver_cur)) {
        ESP_LOGW(TAG, "Non-semver version strings; skipping version check");
        return ESP_OK;
    }

    int cmp = semver_cmp(&ver_new, &ver_cur);

#ifndef CONFIG_OTA_ALLOW_SAME_VERSION
    if (cmp == 0) {
        ESP_LOGI(TAG, "Version unchanged — skipping update");
        return ESP_ERR_NOT_FOUND;
    }
#endif

#ifdef CONFIG_OTA_PREVENT_DOWNGRADE
    if (cmp < 0) {
        ESP_LOGE(TAG, "Refusing downgrade from %s to %s",
                 running->version, incoming->version);
        return ESP_ERR_OTA_VALIDATE_FAILED;
    }
#endif

    return ESP_OK;
}

esp_err_t ota_manager_run(void)
{
    esp_http_client_config_t http_cfg = {
        .url             = CONFIG_OTA_SERVER_URL,
        .cert_pem        = (const char *)server_cert_pem_start,
        .timeout_ms      = CONFIG_OTA_RECV_TIMEOUT_MS,
        .keep_alive_enable = true,
        .skip_cert_common_name_check = false,
    };

    esp_https_ota_config_t ota_cfg = {
        .http_config           = &http_cfg,
        .http_client_init_cb   = NULL,
        .bulk_flash_erase      = false,
        .partial_http_download = false,
    };

    esp_https_ota_handle_t handle = NULL;
    esp_err_t err = esp_https_ota_begin(&ota_cfg, &handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_https_ota_begin: %s", esp_err_to_name(err));
        return err;
    }

    esp_app_desc_t incoming_desc;
    err = esp_https_ota_get_img_desc(handle, &incoming_desc);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_https_ota_get_img_desc: %s", esp_err_to_name(err));
        goto abort;
    }

    err = validate_image_header(&incoming_desc);
    if (err != ESP_OK) {
        goto abort;
    }

    ESP_LOGI(TAG, "Downloading firmware ...");

    do {
        err = esp_https_ota_perform(handle);
        ESP_LOGD(TAG, "Written %d bytes",
                 esp_https_ota_get_image_len_read(handle));
    } while (err == ESP_ERR_HTTPS_OTA_IN_PROGRESS);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_https_ota_perform: %s", esp_err_to_name(err));
        goto abort;
    }

    if (!esp_https_ota_is_complete_data_received(handle)) {
        ESP_LOGE(TAG, "Incomplete firmware image received");
        err = ESP_FAIL;
        goto abort;
    }

    err = esp_https_ota_finish(handle);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_https_ota_finish: %s", esp_err_to_name(err));
        return err;
    }

    ESP_LOGI(TAG, "OTA complete — restarting");
    esp_restart();

    /* Unreachable, but satisfies the compiler. */
    return ESP_OK;

abort:
    esp_https_ota_abort(handle);
    return err;
}

esp_err_t ota_manager_confirm_running_image(void)
{
    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_ota_img_states_t   state;

    esp_err_t err = esp_ota_get_state_partition(running, &state);
    if (err != ESP_OK) {
        return err;
    }

    if (state != ESP_OTA_IMG_PENDING_VERIFY) {
        return ESP_OK;
    }

    ESP_LOGI(TAG, "Confirming OTA image on partition '%s'", running->label);
    return esp_ota_mark_app_valid_cancel_rollback();
}
