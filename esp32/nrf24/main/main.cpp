/*
spi_device_interface_config_t devcfg = {
  .command_bits = 0,
  .address_bits = 0,
  .dummy_bits = 0,
  .mode = 0,
  .clock_source = SPI_CLK_SRC_DEFAULT,
  .clock_speed_hz = 4000000,
  .spics_io_num = SPI_CS_GPIO,
  .queue_size = 1,
};
*/

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static const char *TAG = "nRF24L01 RX";

// ────────────────────────────────────────────────
// Include the library header **BEFORE** using any of its types
// ────────────────────────────────────────────────
extern "C" {
#include "driver_nrf24l01.h"
}

// ────────────────────────────────────────────────
// Pins (adjust to your wiring)
// ────────────────────────────────────────────────
#define PIN_CE GPIO_NUM_14
#define PIN_IRQ GPIO_NUM_15
#define SPI_HOST SPI3_HOST
#define PIN_SCLK GPIO_NUM_18
#define PIN_MISO GPIO_NUM_19
#define PIN_MOSI GPIO_NUM_23
#define PIN_CSN GPIO_NUM_5

static spi_device_handle_t spi_handle = NULL;
static nrf24l01_handle_t nrf = {0};

// ────────────────────────────────────────────────
// Platform interface functions (required by libdriver)
// ────────────────────────────────────────────────
extern "C" {

// SPI init
uint8_t nrf24l01_interface_spi_init(void) {
  spi_bus_config_t buscfg = {
      .mosi_io_num = PIN_MOSI,
      .miso_io_num = PIN_MISO,
      .sclk_io_num = PIN_SCLK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 4096,
  };

  spi_device_interface_config_t devcfg = {
      .command_bits = 0,
      .address_bits = 0,
      .dummy_bits = 0,
      .mode = 0,
      .clock_source = SPI_CLK_SRC_DEFAULT,
      .clock_speed_hz = 10000000,
      .spics_io_num = PIN_CSN,
      .queue_size = 1,
  };

  esp_err_t ret = spi_bus_initialize(SPI_HOST, &buscfg, SPI_DMA_DISABLED);
  if (ret != ESP_OK) {
    return 1;
  }
  ret = spi_bus_add_device(SPI_HOST, &devcfg, &spi_handle);
  return ret == ESP_OK ? 0 : 1;
}

uint8_t nrf24l01_interface_spi_deinit(void) {
  spi_bus_remove_device(spi_handle);
  spi_bus_free(SPI_HOST);
  return 0;
}

uint8_t nrf24l01_interface_spi_write(uint8_t reg, uint8_t *buf, uint16_t len) {
  spi_transaction_t t = {0};
  uint8_t *tx = (uint8_t *)malloc(len + 1);
  if (!tx) {
    return 1;
  }
  tx[0] = reg;
  if (len) {
    memcpy(tx + 1, buf, len);
  }
  t.tx_buffer = tx;
  t.length = (len + 1) * 8;
  esp_err_t ret = spi_device_polling_transmit(spi_handle, &t);
  free(tx);
  return ret == ESP_OK ? 0 : 1;
}

uint8_t nrf24l01_interface_spi_read(uint8_t reg, uint8_t *buf, uint16_t len) {
  if (len == 0) {
    return 0;
  }
  spi_transaction_t t = {0};
  uint8_t *tx = (uint8_t *)malloc(len + 1);
  uint8_t *rx = (uint8_t *)malloc(len + 1);
  if (!tx || !rx) {
    free(tx);
    free(rx);
    return 1;
  }
  tx[0] = reg;
  memset(tx + 1, 0xFF, len);
  t.tx_buffer = tx;
  t.rx_buffer = rx;
  t.length = t.rxlength = (len + 1) * 8;
  esp_err_t ret = spi_device_polling_transmit(spi_handle, &t);
  if (ret == ESP_OK) {
    memcpy(buf, rx + 1, len);
  }
  free(tx);
  free(rx);
  return ret == ESP_OK ? 0 : 1;
}

// CE GPIO
uint8_t nrf24l01_interface_gpio_init(void) {
  gpio_config_t cfg = {
      .pin_bit_mask = (1ULL << PIN_CE),
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };
  return gpio_config(&cfg) == ESP_OK ? 0 : 1;
}

uint8_t nrf24l01_interface_gpio_write(uint8_t value) {
  return gpio_set_level(PIN_CE, value) == ESP_OK ? 0 : 1;
}

void nrf24l01_interface_delay_ms(uint32_t ms) { vTaskDelay(pdMS_TO_TICKS(ms)); }

void nrf24l01_interface_debug_print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
}

// Callback (4 params: type, pipe, buf, len)
void nrf24l01_interface_receive_callback(uint8_t type, uint8_t pipe,
                                         uint8_t *buf, uint8_t len) {
  if (type == NRF24L01_INTERRUPT_RX_DR) {
    ESP_LOGI(TAG, "RX data ready! Pipe %d, %d bytes", pipe, len);
    char hex[3 * 33] = {0};
    int pos = 0;
    for (int i = 0; i < len && pos < 96; i++) {
      pos += snprintf(hex + pos, sizeof(hex) - pos, "%02X ", buf[i]);
    }
    ESP_LOGI(TAG, "Data: %s", hex);
  }
}

uint8_t nrf24l01_interface_gpio_deinit(void) {
  // Nothing to do – or you can add real cleanup if you want
  gpio_reset_pin(PIN_CE);
  return 0;
}

} // extern "C"

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "Initializing nRF24L01...");
  ESP_LOGI(TAG, "RECEIVER mode");

  // IRQ setup
  gpio_config_t irq_cfg = {
      .pin_bit_mask = (1ULL << PIN_IRQ),
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_NEGEDGE,
  };
  ESP_ERROR_CHECK(gpio_config(&irq_cfg));
  ESP_ERROR_CHECK(gpio_install_isr_service(0));

  // Link interfaces
  DRIVER_NRF24L01_LINK_INIT(&nrf, nrf24l01_handle_t);
  DRIVER_NRF24L01_LINK_SPI_INIT(&nrf, nrf24l01_interface_spi_init);
  DRIVER_NRF24L01_LINK_SPI_DEINIT(&nrf, nrf24l01_interface_spi_deinit);
  DRIVER_NRF24L01_LINK_SPI_READ(&nrf, nrf24l01_interface_spi_read);
  DRIVER_NRF24L01_LINK_SPI_WRITE(&nrf, nrf24l01_interface_spi_write);
  DRIVER_NRF24L01_LINK_GPIO_INIT(&nrf, nrf24l01_interface_gpio_init);
  DRIVER_NRF24L01_LINK_GPIO_DEINIT(&nrf, nrf24l01_interface_gpio_deinit);
  DRIVER_NRF24L01_LINK_GPIO_WRITE(&nrf, nrf24l01_interface_gpio_write);
  DRIVER_NRF24L01_LINK_DELAY_MS(&nrf, nrf24l01_interface_delay_ms);
  DRIVER_NRF24L01_LINK_DEBUG_PRINT(&nrf, nrf24l01_interface_debug_print);
  DRIVER_NRF24L01_LINK_RECEIVE_CALLBACK(&nrf,
                                        nrf24l01_interface_receive_callback);

#define CHECK(fn)                                                              \
  do {                                                                         \
    uint8_t res = fn;                                                          \
    if (res != 0) {                                                            \
      ESP_LOGE(TAG, "%s failed: %d", #fn, res);                                \
      nrf24l01_deinit(&nrf);                                                   \
      return;                                                                  \
    }                                                                          \
  } while (0)

  // Initialize
  CHECK(nrf24l01_init(&nrf));

  // ────────────────────────────────────────────────
  // Apply Arduino-like settings
  // ────────────────────────────────────────────────
  uint8_t addr[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

  CHECK(nrf24l01_set_tx_address(&nrf, addr, 5)); // for ACKs
  CHECK(nrf24l01_set_rx_pipe_0_address(&nrf, addr, 5));

  CHECK(nrf24l01_set_address_width(&nrf, NRF24L01_ADDRESS_WIDTH_5_BYTES));

  CHECK(nrf24l01_set_channel_frequency(&nrf, 77));

  CHECK(nrf24l01_set_data_rate(&nrf, NRF24L01_DATA_RATE_250K));

  CHECK(nrf24l01_set_output_power(&nrf, NRF24L01_OUTPUT_POWER_NEGATIVE_18_DBM));

  // Enable pipe 0 + auto ACK on pipe 0
  CHECK(nrf24l01_set_rx_pipe(&nrf, NRF24L01_PIPE_0, NRF24L01_BOOL_TRUE));
  CHECK(nrf24l01_set_auto_acknowledgment(&nrf, NRF24L01_PIPE_0,
                                         NRF24L01_BOOL_TRUE));

  // Static payload 32 bytes on pipe 0
  CHECK(nrf24l01_set_pipe_0_payload_number(&nrf, 32));

  // Disable dynamic payload everywhere
  CHECK(nrf24l01_set_dynamic_payload(&nrf, NRF24L01_BOOL_FALSE));

  // Disable pipes 1–5
  for (int p = 1; p <= 5; p++) {
    CHECK(nrf24l01_set_rx_pipe(&nrf, (nrf24l01_pipe_t)p, NRF24L01_BOOL_FALSE));
    CHECK(nrf24l01_set_auto_acknowledgment(&nrf, (nrf24l01_pipe_t)p,
                                           NRF24L01_BOOL_FALSE));
  }

  // Auto retransmit (ARD=5 → ~1500 µs, ARC=15)
  CHECK(nrf24l01_set_auto_retransmit_delay(&nrf, 5));
  CHECK(nrf24l01_set_auto_retransmit_count(&nrf, 15));

  // 2-byte CRC, power up, RX mode, enable RX_DR interrupt only
  CHECK(nrf24l01_set_config(&nrf, NRF24L01_CONFIG_EN_CRC, NRF24L01_BOOL_TRUE));
  CHECK(nrf24l01_set_config(&nrf, NRF24L01_CONFIG_CRCO,
                            NRF24L01_BOOL_TRUE)); // 2 bytes
  CHECK(nrf24l01_set_config(&nrf, NRF24L01_CONFIG_PWR_UP, NRF24L01_BOOL_TRUE));
  CHECK(nrf24l01_set_mode(&nrf, NRF24L01_MODE_RX));
  CHECK(nrf24l01_set_config(&nrf, NRF24L01_CONFIG_MASK_TX_DS,
                            NRF24L01_BOOL_TRUE));
  CHECK(nrf24l01_set_config(&nrf, NRF24L01_CONFIG_MASK_MAX_RT,
                            NRF24L01_BOOL_TRUE));
  // RX_DR interrupt enabled (MASK_RX_DR = false)

  // Flush & clear status
  nrf24l01_flush_tx(&nrf);
  nrf24l01_flush_rx(&nrf);
  uint8_t status_clear = 0x70;
  nrf24l01_set_reg(&nrf, 0x07, &status_clear, 1); // STATUS register

  nrf24l01_interface_delay_ms(5);
  nrf24l01_interface_gpio_write(1); // CE high → start listening

#undef CHECK

  ESP_LOGI(TAG, "Receiver ready:");
  ESP_LOGI(TAG, "  Address: E7 E7 E7 E7 E7 (pipe 0)");
  ESP_LOGI(TAG, "  Channel: 77");
  ESP_LOGI(TAG, "  Rate:    250 kbps");
  ESP_LOGI(TAG, "  PA:      -18 dBm");
  ESP_LOGI(TAG, "  Payload: 32 bytes static");
  ESP_LOGI(TAG, "Waiting for packets...");

  while (true) {
    // Poll STATUS register (bit 6 = RX_DR)
    uint8_t status = 0;
    nrf24l01_get_reg(&nrf, 0x07, &status, 1);

    if (status & 0x40) { // RX_DR == 1 → data ready
      ESP_LOGI(TAG, "Packet detected (RX_DR set) – reading...");

      // Read payload (library doesn't have direct read in polling, so manual)
      uint8_t payload[32];
      uint8_t width = 0;
      nrf24l01_get_rx_payload_width(&nrf, &width);
      if (width <= 32) {
        nrf24l01_read_rx_payload(&nrf, payload, width);
        char hex[3 * 33] = {0};
        int pos = 0;
        for (int i = 0; i < width; i++) {
          pos += snprintf(hex + pos, sizeof(hex) - pos, "%02X ", payload[i]);
        }
        ESP_LOGI(TAG, "Received %d bytes: %s", width, hex);

        char msg[33] = {0};
        int start = 0;
        while (start < width &&
               (payload[start] == 0 || payload[start] == ' ')) {
          start++;
        }
        memcpy(msg, payload + start, width - start);
        msg[width - start] = '\0';

        // Reverse it if needed (your case seems reversed)
        for (int i = 0; i < (width - start) / 2; i++) {
          char tmp = msg[i];
          msg[i] = msg[width - start - 1 - i];
          msg[width - start - 1 - i] = tmp;
        }

        ESP_LOGI(TAG, "Received message: \"%s\"", msg);
      }

      // Clear RX_DR bit by writing to STATUS
      uint8_t clear_rx_dr = 0x40;
      nrf24l01_set_reg(&nrf, 0x07, &clear_rx_dr, 1);
    }

    // Optional: show FIFO status for debugging
    uint8_t fifo = 0;
    nrf24l01_get_reg(&nrf, 0x17, &fifo, 1);
    if (fifo & 0x02) { // RX_FULL
      ESP_LOGW(TAG, "RX FIFO full – flushing");
      nrf24l01_flush_rx(&nrf);
    }

    vTaskDelay(pdMS_TO_TICKS(50)); // Poll ~20 times per second
  }
}
