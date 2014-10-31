#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <stdint.h>

#define UTIL_ONE_SECOND             168000
#define UTIL_STM32_CLOCK_HZ         168000000UL
#define UTIL_STM32_CYCLES_PER_LOOP  10
#define TRUE                        1
#define FALSE                       0

// led macros
#define UTIL_GPIO_LED               GPIOD
#define UTIL_GPIO_CLOCK             RCC_AHB1Periph_GPIOD
#define UTIL_LED1_PIN               GPIO_Pin_12
#define UTIL_LED2_PIN               GPIO_Pin_13
#define UTIL_LED3_PIN               GPIO_Pin_14
#define UTIL_LED4_PIN               GPIO_Pin_15

void util_leds_init();
void util_delay(uint32_t tick);
void util_delay_ms(const uint32_t ms);

#endif /* UTIL_H_INCLUDED */
