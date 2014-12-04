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
#include "stm32f4xx_tim.h"
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

    GPIO_ToggleBits(GPIO_LED, LED2_PIN);

    global_init();
    pwm_init();
    i2c_init();
    imu_configure_gyro();
    imu_configure_accelerometer();
    imu_configure_mag();
    util_delay_ms(3000);
    int_init();

//    int right_pwm = PULSE_ONE_MS;
//    int left_pwm = PULSE_ONE_MS;
//    printf("%d\n", left_pwm);
//    util_delay_ms(5000);
//    pwm_inc_to_value(&left_pwm, PULSE_ONE_MS+1000, &LEFT_MOTOR_FUNC);
//    util_delay_ms(5000);
//    pwm_inc_to_value(&left_pwm, PULSE_ONE_MS+500, &LEFT_MOTOR_FUNC);
    while (1) {
//        printf("gyro: %d %d %d\n", (int)g_gyro.x_ang, (int)g_gyro.y_ang, (int)g_gyro.z_ang);
//        printf("mag: %d %d %d\n", (int)mag.x, (int)mag.y, (int)mag.z);
//        printf("accel: %d %d %d\n", (int)accel.x_ang, (int)accel.y_ang, (int)accel.z_ang);
//        printf("comp: %d %d %d\n", g_ang.comp_x, g_ang.comp_y, g_ang.comp_z);
    }
}
