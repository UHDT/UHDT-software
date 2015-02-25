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
#include "states.h"

#define MAIN_FILE
#include "globals.h"




int main(void)
{	int test = 1;
	int increment = 1;
	printf("step: %d\n", (int)test);
    SystemInit();
    test += increment;
	printf("step: %d\n", (int)test);


    util_delay_init();
    util_delay_enable_systick();
    util_leds_init();
    test += increment;
	printf("step: %d\n", (int)test);

    GPIO_ToggleBits(GPIO_LED, LED2_PIN);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    GPIO_InitTypeDef GPIO_InitDef; //Where GPIO_InitDef is variable to work with struct
    global_init();
    pwm_init();
    i2c_init();

    test += increment;
	printf("step: %d\n", (int)test);
    imu_configure_gyro();
    imu_configure_accelerometer();
    imu_configure_mag();
    test += increment;
	printf("step: %d\n", (int)test);
    util_delay_ms(1000);
    test += increment;
	printf("step: %d\n", (int)test);

//debug
/*
int a = 1;
int b = 2;
int c = 3;
int d = 4;
int e = 5;
int f = 6;
int g = 7;
int h = 8;
int i = 9;

while(1)
{
	states(&a,&b,&c,&d,&e,&f,&g,&h,&i);
	printf(" %d %d %d %d %d %d %d %d %d\n", a,b,c,d,e,f,g,h,i);
}
*/
// end debug
    int_init();
    test += increment;
	printf("step: %d\n", (int)test);

    while (1) {
//        printf("gyro: %d %d %d\n", (int)g_gyro.x_ang, (int)g_gyro.y_ang, (int)g_gyro.z_ang);
//        printf("mag: %d %d %d\n", (int)mag.x, (int)mag.y, (int)mag.z);
//        printf("accel: %d %d %d\n", (int)accel.x_ang, (int)accel.y_ang, (int)accel.z_ang);
//        printf("comp: %d %d %d\n", g_ang.comp_x, g_ang.comp_y, g_ang.comp_z);
    }
}

