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
    g_pwm_value = PWM_ONE_MS;
    int count = 0;
    int increment = PWM_INCREMENT;
    while (1) {
        pwm_increment_motor(increment, 10, 10);
        util_delay_ms(1000*5);
        printf("%d\n", g_pwm_value);
        count++;
        if (count % 3 == 0) {
            if (increment == PWM_INCREMENT) {
                increment = PWM_DECREMENT;
            } else {
                increment = PWM_INCREMENT;
            }
        }
    }
}
