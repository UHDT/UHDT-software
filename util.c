#include "util.h"

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
