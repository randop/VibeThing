// ====================================================================
// Minimal blink example for CHEAP CHINESE BLUE PILL (STM32F103C8T6 clone)
// LED on PC13 – active LOW (0 = ON, 1 = OFF)
// Uses internal 8 MHz HSI oscillator by default
// ====================================================================

#define RCC_BASE    0x40021000U
#define GPIOC_BASE  0x40011000U

#define RCC_APB2ENR     (*(volatile unsigned int*)(RCC_BASE   + 0x00 + 0x18))
#define GPIOC_CRL       (*(volatile unsigned int*)(GPIOC_BASE + 0x00))
#define GPIOC_ODR       (*(volatile unsigned int*)(GPIOC_BASE + 0x0C))

#define RCC_APB2ENR_IOPCEN  (1U << 4)   // GPIOC clock enable

#define LED_PORT    GPIOC
#define LED_PIN     13

// Very rough delay (~ 1 second @ 8 MHz HSI)
static void delay_approx_1s(void)
{
    volatile unsigned int i;
    for (i = 0; i < 800000; i++) { /* empty loop */ }
}

int main(void)
{
    // Enable GPIOC clock
    RCC_APB2ENR |= RCC_APB2ENR_IOPCEN;

    // Configure PC13 as output 2 MHz push-pull
    GPIOC_CRL &= ~(0xF << 20);         // Clear configuration bits for PC13
    GPIOC_CRL |=  (0x2 << 20);         // Output mode 2 MHz, push-pull

    // Endless loop
    while (1)
    {
        GPIOC_ODR &= ~(1U << 13);     // LED ON  (PC13 = 0)
        delay_approx_1s();

        GPIOC_ODR |=  (1U << 13);     // LED OFF (PC13 = 1)
        delay_approx_1s();
    }

    // Never reached
    return 0;
}
