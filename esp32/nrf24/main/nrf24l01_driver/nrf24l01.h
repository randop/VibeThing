#ifndef NRF24L01_H_
#define NRF24L01_H_

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>

#ifdef __cplusplus
extern "C" {
#endif

// === CONFIGURABLE PINS ===
#define NRF24_SPI_HOST      SPI2_HOST
#define NRF24_PIN_MISO      19
#define NRF24_PIN_MOSI      23
#define NRF24_PIN_SCK       18
#define NRF24_PIN_CSN       5
#define NRF24_PIN_CE        14
// #define NRF24_PIN_IRQ     27   // optional

esp_err_t nrf24_init(void);
esp_err_t nrf24_send(const uint8_t *data, uint8_t len);
bool      nrf24_is_data_available(void);
esp_err_t nrf24_read(uint8_t *buffer, uint8_t *length);

#ifdef __cplusplus
}
#endif

#endif /* NRF24L01_H_ */
