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
#include <stdio.h>
#include "stm32f4xx_conf.h"
#include "pwm.h"
#include "util.h"
#include "interrupts.h"
#include "i2c.h"
#include "imu.h"
#include "macros.h"

#define MAIN_FILE
#include "globals.h"

int main(void)
{
    SystemInit();
    util_delay_init();
    util_leds_init();
    pwm_init();
    i2c_init();
    imu_configure_gyro();
    imu_configure_accelerometer();
    imu_configure_mag();
    int_init();

    Mag mag;
    Accel accel;
    Gyro gyro;
    Angle ang;
    gyro.x_ang = 0;
    gyro.y_ang = 0;
    gyro.z_ang = 0;
    while (1) {
        GPIO_ToggleBits(GPIO_LED, LED2_PIN);
        printf("gyro: %d %d %d\n", (int)g_gyro.x_ang, (int)g_gyro.y_ang, (int)g_gyro.z_ang);
        util_delay_ms(1000);
//        printf("mag: %d %d %d\n", (int)mag.x, (int)mag.y, (int)mag.z);
//        printf("accel: %d %d %d\n", (int)accel.x_ang, (int)accel.y_ang, (int)accel.z_ang);
    }
}

/*
int main(void)
{
    // Initialize system
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
//    pwm_percent_motor(&front_pwm, 100);
//    pwm_percent_motor(&front_pwm, 0);
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
*/
