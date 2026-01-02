#if defined STM32L0
#include <stm32l0xx_hal.h>

// STM32L0538-Discovery green led - PB4
#define LED_PORT GPIOB
#define LED_PIN GPIO_PIN_4
#define LED_PORT_CLK_ENABLE __HAL_RCC_GPIOB_CLK_ENABLE
#elif defined STM32F1
#include <stm32f1xx_hal.h>

// STM32VL-Discovery green led - PC9
// Blue Pill (STM32F103C8T6) onboard green led - PC13 (active-low)
#define LED_PORT GPIOC
#define LED_PIN GPIO_PIN_13
#define LED_PORT_CLK_ENABLE __HAL_RCC_GPIOC_CLK_ENABLE
#elif defined STM32F4
#include <stm32f4xx_hal.h>

// STM32F4-Discovery green led - PD12
#define LED_PORT GPIOD
#define LED_PIN GPIO_PIN_12
#define LED_PORT_CLK_ENABLE __HAL_RCC_GPIOD_CLK_ENABLE
#endif

void SysTick_Handler(void) {
  HAL_IncTick();
  // No work here to make interrupt very short and responsive
}

void initGPIO() {
  GPIO_InitTypeDef GPIO_Config;

  GPIO_Config.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Config.Pull = GPIO_NOPULL;
  GPIO_Config.Speed = GPIO_SPEED_FREQ_LOW; // <-- Change to LOW (safer for PC13)

  GPIO_Config.Pin = LED_PIN;

  LED_PORT_CLK_ENABLE();
  HAL_GPIO_Init(LED_PORT, &GPIO_Config);

  // Explicitly set LED ON immediately after initialization
  HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

int main(void) {
  HAL_Init();
  initGPIO();

  // 1kHz ticks
  HAL_SYSTICK_Config(SystemCoreClock / 1000);

  HAL_Delay(3000);

  // Timing for 3-second period blink (1.5 s on, 1.5 s off)
  // Since we start with LED ON, first toggle turns it OFF after 1.5 s
  uint32_t led_next_toggle = HAL_GetTick() + 1500;

  while (1) {
    uint32_t current_tick = HAL_GetTick();

    // Toggle every 1.5 seconds
    if (current_tick >= led_next_toggle) {
      HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
      led_next_toggle += 1500; // Next toggle in another 1.5 s
    }
  }

  return 0;
}
