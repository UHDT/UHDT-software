/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f4xx_conf.h"
#include "pwm.h"
#include "util.h"

#define MAIN_FILE
#include "globals.h"

int main(void)
{
    /* Initialize system */
    SystemInit();
    /* Init leds */
    pwm_timer_init( );
    /* Init timer */
    pwm_init();
    /* Init PWM */
    pwm_leds_init();
    util_delay_ms(1000*5);
//    util_delay(UTIL_ONE_SECOND*20);
    g_pwm_value = PWM_ONE_MS;
    pwm_run_motor(50);
    while (1) {
    }
}
