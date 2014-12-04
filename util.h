#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <stdint.h>
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"

static __IO uint32_t util_timing_delay;
/**
 * This variable can be used in main
 * It is automatically increased every time systick make an interrupt
 */
static __IO uint32_t util_time;
static __IO uint32_t util_time2;

void util_leds_init();

/*
How to use Timer:
int main()
{
    SystemInit();
    util_delay_init();
    // if you want a straight delay (1s)
    util_delay_ms(1000);
    // if you want to use like an interrupt
    util_delay_set_time(0); // resets the counter to 0
    while (1)
    {
        if (util_delay_time() >= 500000)
        {
            util_delay_set_time(0);
            GPIO_ToggleBits(GPIO_LED, LED1_PIN);
        }
    }
}
*/
void util_delay_init(void);
void util_delay_ms(__IO uint32_t nTime);
void util_delay_ns(__IO uint32_t nTime);
uint32_t util_delay_time(void);
void util_delay_set_time(uint32_t time);
uint32_t util_delay_time2(void);
void util_delay_set_time2(uint32_t time);
void util_delay_enable_systick(void);
void util_delay_disable_systick(void);

#endif /* UTIL_H_INCLUDED */
