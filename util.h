#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <stdint.h>

#define UTIL_ONE_SECOND             168000
#define UTIL_STM32_CLOCK_HZ         168000000UL
#define UTIL_STM32_CYCLES_PER_LOOP  10
#define TRUE                        1
#define FALSE                       0

void util_delay(uint32_t tick);
void util_delay_ms(const uint32_t ms);

#endif /* UTIL_H_INCLUDED */
