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

    util_delay(1680000*3);
    int pwm_value = 4400;
    while (1) {
        if (pwm_value < 5500) {
            TIM_SetCompare1(TIM4, pwm_value);
            pwm_value++;
        }
    }
}
