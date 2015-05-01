#ifndef INTERRUPTS_H_INCLUDED
#define INTERRUPTS_H_INCLUDED

#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include "protocol.h"
#include "xbee.h"
#include "macros.h"
#include <stdint.h>

#ifdef MAIN
// g_transmit_time is the amount of time in milli-seconds (ms)
// between each packet transmission. Value must be a multiple
// of 100 because interrupt happens every 100ms. Currently,
// lowest value for g_transmit_time is 300. Any lower causes
// hard faults.
int g_transmit_time;
#else
extern int g_transmit_time;
#endif
void int_init();
void int_init_timer();
void int_enable_interrupt();

#endif /* INTERRUPTS_H_INCLUDED */
