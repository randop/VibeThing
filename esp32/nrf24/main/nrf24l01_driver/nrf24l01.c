#include "nrf24l01.h"
#include "nrf24l01_regs.h"
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>

static const char *TAG = "NRF24";
static spi_device_handle_t spi_handle = NULL;

static inline void csn_low(void) { gpio_set_level(NRF24_PIN_CSN, 0); }
static inline void csn_high(void) {
  gpio_set_level(NRF24_PIN_CSN, 1);
  esp_rom_delay_us(200); // 200 µs — critical for many cheap modules
}
static inline void ce_low(void) { gpio_set_level(NRF24_PIN_CE, 0); }
static inline void ce_high(void) { gpio_set_level(NRF24_PIN_CE, 1); }

// Helper: Execute a full blocking SPI transaction (start + end)
static esp_err_t nrf24_spi_execute(spi_transaction_t *t) {
  esp_err_t ret = spi_device_polling_start(spi_handle, t, portMAX_DELAY);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "SPI polling start failed: %s", esp_err_to_name(ret));
    return ret;
  }

  ret = spi_device_polling_end(spi_handle, portMAX_DELAY);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "SPI polling end failed: %s", esp_err_to_name(ret));
    return ret;
  }

  return ESP_OK;
}

static uint8_t nrf24_read_reg(uint8_t reg) {
  uint8_t tx[2] = {R_REGISTER | (reg & 0x1F), 0xFF};
  uint8_t rx[2] = {0};

  spi_transaction_t t = {.length = 16, .tx_buffer = tx, .rx_buffer = rx};

  csn_low();
  nrf24_spi_execute(&t);
  csn_high();

  return rx[1];
}

static void nrf24_write_reg(uint8_t reg, uint8_t value) {
  uint8_t tx[2] = {W_REGISTER | (reg & 0x1F), value};

  spi_transaction_t t = {.length = 16, .tx_buffer = tx};

  csn_low();
  nrf24_spi_execute(&t);
  csn_high();
}

static void nrf24_read_buf(uint8_t reg, uint8_t *buf, uint8_t len) {
  uint8_t cmd = R_REGISTER | (reg & 0x1F);

  spi_transaction_t cmd_t = {.length = 8, .tx_buffer = &cmd};

  spi_transaction_t data_t = {.length = len * 8, .rx_buffer = buf};

  csn_low();
  nrf24_spi_execute(&cmd_t);
  nrf24_spi_execute(&data_t);
  csn_high();
}

static void nrf24_write_buf(uint8_t reg, const uint8_t *data, uint8_t len) {
  uint8_t cmd = W_REGISTER | (reg & 0x1F);

  spi_transaction_t cmd_t = {.length = 8, .tx_buffer = &cmd};

  spi_transaction_t data_t = {.length = len * 8, .tx_buffer = data};

  csn_low();
  nrf24_spi_execute(&cmd_t);
  nrf24_spi_execute(&data_t);
  csn_high();
}

// ──────────────────────────────────────────────────────────────

esp_err_t nrf24_init(void) {
  // GPIO config for CSN & CE
  gpio_config_t io_conf = {.pin_bit_mask =
                               (1ULL << NRF24_PIN_CSN) | (1ULL << NRF24_PIN_CE),
                           .mode = GPIO_MODE_OUTPUT,
                           .pull_up_en = 0,
                           .pull_down_en = 0,
                           .intr_type = GPIO_INTR_DISABLE};
  gpio_config(&io_conf);

  csn_high();
  ce_low();

  // SPI bus initialization
  spi_bus_config_t buscfg = {
      .miso_io_num = NRF24_PIN_MISO,
      .mosi_io_num = NRF24_PIN_MOSI,
      .sclk_io_num = NRF24_PIN_SCK,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .max_transfer_sz = 0,
  };

  ESP_ERROR_CHECK(spi_bus_initialize(NRF24_SPI_HOST, &buscfg, SPI_DMA_CH_AUTO));

  spi_device_interface_config_t devcfg = {
      .clock_speed_hz = 1 * 1000 * 1000,
      .mode = 0,
      .spics_io_num = -1, // manual CSN control
      .queue_size = 4,
  };

  ESP_ERROR_CHECK(spi_bus_add_device(NRF24_SPI_HOST, &devcfg, &spi_handle));

  vTaskDelay(pdMS_TO_TICKS(5)); // >1.5ms after power-up

  // Software reset & common stable config
  nrf24_write_reg(CONFIG, 0x0C); // power down
  vTaskDelay(pdMS_TO_TICKS(5));

  nrf24_write_reg(EN_AA, 0x01);                    // Enable auto-ack pipe 0
  nrf24_write_reg(EN_RXADDR, 0x01);                // Enable data pipe 0
  nrf24_write_reg(SETUP_AW, 0x03);                 // 5 byte address
  nrf24_write_reg(SETUP_RETR, (0x0F << 4) | 0x03); // 1500µs, 15 retries

  // IMPORTANT: must match
  nrf24_write_reg(RF_CH, 76);

  // 250 kbps + PA_LOW
  nrf24_write_reg(RF_SETUP, 0x27);

  nrf24_write_reg(STATUS, 0x70); // clear all interrupts
  nrf24_write_reg(DYNPD, 0x00);
  nrf24_write_reg(FEATURE, 0x00);

  // IMPORTANT: must match
  uint8_t addr[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};

  nrf24_write_buf(TX_ADDR, addr, 5);
  nrf24_write_buf(RX_ADDR_P0, addr, 5);

  // Power up in RX mode + CRC 2 bytes
  nrf24_write_reg(CONFIG,
                  (1 << PWR_UP) | (1 << PRIM_RX) | (1 << EN_CRC) | (1 << CRCO));
  esp_rom_delay_us(100); // small delay after each write

  // Switch to TX mode (PRIM_RX = 0), power up, enable CRC
  // uint8_t config = nrf24_read_reg(CONFIG);
  // config &= ~(1 << PRIM_RX);                      // TX mode
  // config |= (1 << PWR_UP) | (1 << EN_CRC) | (1 << CRCO);
  // nrf24_write_reg(CONFIG, config);
  ce_high();
  esp_rom_delay_us(500); // or even 500 for extreme test
  ce_low();

  vTaskDelay(pdMS_TO_TICKS(1));
  uint8_t config_verify = nrf24_read_reg(CONFIG);
  ESP_LOGI(TAG, "CONFIG verify after PWR_UP = 0x%02X (expect ~0x3E)",
           config_verify);

  ESP_LOGI(TAG, "nRF24L01 initialized (channel %d, 250kbps)", 115);

  // ESP_LOGI(TAG, "nRF24L01 initialized (SPI 4MHz, channel 76, 1Mbps)");
  return ESP_OK;
}

esp_err_t nrf24_send(const uint8_t *data, uint8_t len) {
  if (len == 0 || len > 32) {
    return ESP_ERR_INVALID_ARG;
  }

  // Clear all interrupt flags before starting TX
  nrf24_write_reg(STATUS, 0x70);
  // nrf24_write_reg(EN_AA, 0x00);  // ← disable auto ack

  // Enable pipe 0 (for ACK)
  nrf24_write_reg(EN_RXADDR, 0x01);

  // Enable auto-ack on pipe 0
  nrf24_write_reg(EN_AA, 0x01);

  ce_high();
  esp_rom_delay_us(500); // or even 500 for extreme test
  ce_low();

  nrf24_write_buf(W_TX_PAYLOAD, data, len);
  ESP_LOGI(TAG, "Payload loaded");

  // Wait for completion
  TickType_t start = xTaskGetTickCount();
  int loop_count = 0;
  while (1) {
    uint8_t status = nrf24_read_reg(STATUS);
    ESP_LOGI(TAG, "Wait loop %d - STATUS = 0x%02x", ++loop_count, status);

    if (status & (1 << TX_DS)) {
      nrf24_write_reg(STATUS, (1 << TX_DS) | (1 << MAX_RT));
      ESP_LOGD(TAG, "TX success - STATUS: 0x%02x", status);
      return ESP_OK;
    }

    if (status & (1 << MAX_RT)) {
      nrf24_write_reg(STATUS, (1 << TX_DS) | (1 << MAX_RT));
      nrf24_write_reg(FLUSH_TX, 0xFF); // drop stuck packet
      ESP_LOGW(TAG, "TX max retries - STATUS: 0x%02x", status);
      return ESP_FAIL;
    }

    if ((xTaskGetTickCount() - start) > pdMS_TO_TICKS(500)) {
      ESP_LOGE(TAG, "TX timeout - STATUS: 0x%02x", status);
      ESP_LOGE(TAG, "TX TIMEOUT after %d checks", loop_count);
      nrf24_write_reg(FLUSH_TX, 0xFF);
      return ESP_ERR_TIMEOUT;
    }

    vTaskDelay(pdMS_TO_TICKS(5));
  }
}

bool nrf24_is_data_available(void) {
  return (nrf24_read_reg(STATUS) & (1 << RX_DR)) != 0;
}

esp_err_t nrf24_read(uint8_t *buffer, uint8_t *length) {
  uint8_t status = nrf24_read_reg(STATUS);

  if (!(status & (1 << RX_DR))) {
    return ESP_ERR_NOT_FOUND;
  }

  // Get payload length from pipe
  uint8_t pipe = (status >> 1) & 0x07;
  uint8_t payload_len = nrf24_read_reg(RX_PW_P0 + pipe);

  if (payload_len > 32) {
    payload_len = 32;
  }

  nrf24_read_buf(R_RX_PAYLOAD, buffer, payload_len);
  *length = payload_len;

  // Clear RX_DR flag
  nrf24_write_reg(STATUS, (1 << RX_DR));

  ESP_LOGD(TAG, "RX success - pipe: %d, len: %d", pipe, payload_len);
  return ESP_OK;
}

uint8_t nrf24_read_status(uint8_t reg) {
  uint8_t tx[2] = {R_REGISTER | (reg & 0x1F), 0xFF};
  uint8_t rx[2] = {0};

  spi_transaction_t t = {.length = 16, .tx_buffer = tx, .rx_buffer = rx};

  csn_low();
  nrf24_spi_execute(
      &t); // or spi_device_polling_transmit(...) if you still use that
  csn_high();

  return rx[1];
}
