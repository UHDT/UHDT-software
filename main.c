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
    pwm_init();
    util_leds_init();
    util_delay_ms(1000*5);
    int front_pwm = PWM_ONE_MS;
    int count = 0;
    int increment = PWM_INCREMENT;
    GPIO_SetBits(UTIL_GPIO_LED, UTIL_LED1_PIN);
    GPIO_SetBits(UTIL_GPIO_LED, UTIL_LED2_PIN);
    GPIO_SetBits(UTIL_GPIO_LED, UTIL_LED3_PIN);
    GPIO_SetBits(UTIL_GPIO_LED, UTIL_LED4_PIN);
    while (1) {
        pwm_increment_motor(&front_pwm, increment, 10);
        util_delay_ms(1000*5);
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
