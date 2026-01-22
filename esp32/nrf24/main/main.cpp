#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nrf24l01.h"
#include "nrf24l01_regs.h"
#include <stdio.h>
#include <string.h>

static const char *TAG = "PROGRAM_nRF24L01";

static void sender_task(void *pvParameters) {
  uint8_t counter = 0;
  while (1) {
    char message[48];
    snprintf(message, sizeof(message), "Sending message #%03u", counter++);

    esp_err_t res = nrf24_send((uint8_t *)message, strlen(message) + 1);

    ESP_LOGI(TAG, "Sent: %s", res == ESP_OK ? "OK" : "FAIL");

    vTaskDelay(pdMS_TO_TICKS(4000));
  }
}

static void receiver_task(void *pvParameters) {
  uint8_t buffer[33];
  uint8_t len;
  esp_rom_delay_us(50);

  ESP_LOGI(TAG, "nRF24L01 receiving...");

  while (1) {
    if (nrf24_is_data_available()) {
      if (nrf24_read(buffer, &len) == ESP_OK) {
        buffer[len] = '\0';
        ESP_LOGI(TAG, "Received (%u bytes): %s", len, buffer);
      }
    } else {
      // Optional: log every few seconds to see if anything changes
      static uint32_t last = 0;
      if (xTaskGetTickCount() - last > pdMS_TO_TICKS(3000)) {
        uint8_t status = nrf24_read_status(STATUS);
        ESP_LOGI("RX", "Idle poll - STATUS=0x%02x", status);
        last = xTaskGetTickCount();
      }
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

extern "C" void app_main(void) {
  esp_err_t ret = nrf24_init();
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "nRF24L01 initialization failed!");
    vTaskDelay(portMAX_DELAY);
  }

  ESP_LOGI(TAG, "Starting tasks...");

  // xTaskCreatePinnedToCore(sender_task,   "sender",   3072, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(receiver_task, "receiver", 3072, NULL, 5, NULL, 0);
}
