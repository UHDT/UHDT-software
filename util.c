#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "util.h"

void util_leds_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // initiliaze clock for leds
    RCC_AHB1PeriphClockCmd(UTIL_GPIO_CLOCK, ENABLE);

    // set pins for leds
    GPIO_InitStruct.GPIO_Pin = UTIL_LED1_PIN;// | UTIL_LED2_PIN | UTIL_LED3_PIN | UTIL_LED4_PIN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(UTIL_GPIO_LED, &GPIO_InitStruct);
}

void util_delay(uint32_t ticks)
{
    uint32_t i = 0;
    for (i = 0; i < ticks; i++)
    {
        SystemCoreClockUpdate();
    }
}

void util_delay_ms(uint32_t ms)
{
    ms *= UTIL_STM32_CLOCK_HZ / 1000 / UTIL_STM32_CYCLES_PER_LOOP;

    asm volatile(" mov r0, %[ms] \n\t"
            "1: subs r0, #1 \n\t"
             " bhi 1b \n\t"
             :
             : [ms] "r" (ms)
             : "r0");
}


