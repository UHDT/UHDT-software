#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include "macros.h"
#include "util.h"
#include "interrupts.h"
#include "imu.h"
#include "globals.h"
#include "pwm.h"
#include "stm32f4xx_gpio.h"
#include "states.h"

// Initializes the interrupts. This means that we set up
// the proper timer for the interrupts and enable it.
// @param: none
// @return: none
void int_init()
{	int test = 1;
printf("test %d\n",(int)test);
//	gpio_init();
    int_init_timer();
    int_enable_interrupt();
}

// Sets up the timer that will cause the interrupt. In this function,
// the frequency of interrupts is determined.
// @param: none
// @return: none
void int_init_timer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_BaseStructure;
    // frequency = (clockspeed/2) / prescaler / period
    // (180000000/2) / 45000 / 100 = 20Hz
    TIM_BaseStructure.TIM_Prescaler = 45000;
    TIM_BaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStructure.TIM_Period = 100;
    TIM_BaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_BaseStructure);
    TIM_Cmd(TIM2, ENABLE);
}

// Sets the priority for the interrupt of timer 2. Also starts
// the timer.
// @param: none
// @return: none
void int_enable_interrupt()
{
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

// Timer2 interrupt handler. This is what happens when the timer2
// goes off.
// Get the data from the IMU, calculates the proper PID adjustments,
// then increments the corresponding motors.
// @param: none
// @return: none
void TIM2_IRQHandler()
{
    static int counter = 0;
    // previous roll value
    static int prev_roll = 0;
    // check to see if interrupt happened
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // reset the timer flag --> you HAVE to do this or else it will keep interrupting
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        GPIO_ToggleBits(GPIO_LED, LED1_PIN);

        // get all necessary data and filter it
        imu_fill_gyro_data(&g_gyro);
        imu_fill_mag_data(&g_mag);
        imu_fill_accel_data(&g_accel);
        // get the angle data that will be used for PID controller
        imu_fill_angle_data(&g_ang, &g_gyro, &g_mag, &g_accel);
        // p value for roll
        int p_value = (g_ang.comp_x - g_roll_setpoint) * P_ROLL;
        // d value for roll
        int d_value = (g_ang.comp_x - prev_roll) * D_ROLL;
//        printf("%d\n", d_value);


        // this is the base pwm value. When set, determines how fast
        // the motors will be spinning at rest

        int back_motor = g_back_motor.PULSE_VALUE + 1000;
        int left_motor = 0;
        int right_motor = 0;

        int save1 = 0;
        int save2 = 0;
        int save3 = 0;

        // increment or decrement each motors value
        //left_motor += p_value + d_value;
        //right_motor -= p_value + d_value;

        // cap off the values
    //    util_cap_value(&left_motor, g_left_motor.PULSE_VALUE, g_left_motor.PULSE_VALUE*2);
   //     util_cap_value(&right_motor, g_right_motor.PULSE_VALUE, g_right_motor.PULSE_VALUE*2);

        printf("test%d\n", counter);
        printf("motor: %d %d %d \n", (int)left_motor, (int)right_motor, (int)back_motor);

// states
        int PIN1 = GPIO_Pin_10; // enable 1
        int PIN2 = GPIO_Pin_11;// input 1
        int PIN3 = GPIO_Pin_12;// enable 2
        int PIN4 = GPIO_Pin_13;// input 2
        int PIN5 = GPIO_Pin_14;// enable 3
        int PIN6 = GPIO_Pin_15;// input 3
        int PIN7 = GPIO_Pin_12;

// enable states
        int state1 = PIN2;
        int state2 = PIN4;
        int state3 = PIN6;

        printf("%d %d %d %d %d %d %d %d %d\n", PIN1, PIN3, PIN5, PIN2, state1, PIN4, state2, PIN6, state3);

// Direction
        int forward = 1;

while(1)
{
	save1 = back_motor;
	save2 = left_motor;
	save3 = right_motor;
    back_motor = save3;
    left_motor = save1;
    right_motor = save2;

// delay
    util_delay_ms(100);

// Enable
    GPIO_SetBits(GPIOD,PIN3 | PIN5 |PIN1);
    GPIO_ResetBits(GPIOB,PIN1 );

// Input
    GPIO_SetBits(GPIOB,PIN2);
    GPIO_SetBits(GPIOD,PIN4 );
    GPIO_ResetBits(GPIOD,PIN6 );


    if(forward == 1)
    {
    	states(&PIN1,&PIN3,&PIN5,&PIN2,&state1,&PIN4,&state2,&PIN6,&state3);
    	printf("%d %d %d %d %d %d %d %d %d", PIN1, PIN3, PIN5, PIN2, state1, PIN4, state2, PIN6, state3);
    }
    else
    {
    	states(&PIN1,&PIN5,&PIN3,&state3,&PIN6,&state2,&PIN6,&state1,&PIN2);
    	//printf("%d %d %d %d %d %d %d %d %d", PIN1, PIN3, PIN5, PIN2, state1, PIN4, state2, PIN6, state3);
	}


//	left_motor = back_motor;
//	back_motor = right_motor;
//  right_motor = left_motor;


        // change the motor speeds
        pwm_inc_to_value(&g_left_motor, left_motor);
        pwm_inc_to_value(&g_right_motor, right_motor);
//        pwm_inc_to_value(&g_front_motor, front_motor);
        pwm_inc_to_value(&g_back_motor, back_motor);

/*        if (counter >= 3)
        {
            prev_roll = g_ang.comp_x;
            counter = 0;
        }
        counter++;
*/
 //debug printf
 //        		printf("gyro: %d %d %d\t", (int)g_gyro.x_ang, (int)g_gyro.y_ang, (int)g_gyro.z_ang);
 //               printf("accel: %d %d %d\t", (int)g_accel.x_ang, (int)g_accel.y_ang, (int)g_accel.z_ang);
 //               printf("angle: %d %d %d\t", (int)g_ang.comp_x, (int)g_ang.comp_y, (int)g_ang.comp_z);
//                printf("motor: %d %d %d \t", (int)left_motor, (int)right_motor, (int)back_motor);
//                printf("save: %d %d %d \n", (int)save1, (int)save2, (int)save3);

                //               printf("pvalue: %d dvalue: %d\n", (int)p_value, (int)d_value);
 //end debug
}
    }
 }

void gpio_init()
{	int test = 2;
	printf("test %d\n",(int)test);
	GPIO_InitTypeDef GPIO_InitDef;
	    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	    GPIO_InitDef.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	    GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	    //Initialize pins

	    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	    GPIO_InitDef.GPIO_Pin = GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
   	    GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
   	    GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
   	    GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
   	    GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
}

