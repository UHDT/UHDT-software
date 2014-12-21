#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include "macros.h"
#include "util.h"
#include "interrupts.h"
#include "imu.h"
#include "globals.h"
#include "pwm.h"

// Initializes the interrupts. This means that we set up
// the proper timer for the interrupts and enable it.
// @param: none
// @return: none
void int_init()
{
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
    static int prev_roll = g_roll_setpoint;
    // check to see if interrpt happened
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
        printf("%d\n", d_value);

        // this is the base pwm value. When set, determines how fast
        // the motors will be spinning at rest
        int left_motor = g_left_motor.PULSE_VALUE + 1000;
        int right_motor = g_right_motor.PULSE_VALUE + 1000;

        // increment or decrement each motors value
        left_motor += p_value + d_value;
        right_motor -= p_value + d_value;

        // cap off the values
        util_cap_value(&left_motor, g_left_motor.PULSE_VALUE, g_left_motor.PULSE_VALUE*2);
        util_cap_value(&right_motor, g_right_motor.PULSE_VALUE, g_right_motor.PULSE_VALUE*2);

        // change the motor speeds
        pwm_inc_to_value(&g_left_motor, left_motor);
        pwm_inc_to_value(&g_right_motor, right_motor);
        if (counter >= 3)
        {
            prev_roll = g_ang.comp_x;
            counter = 0;
        }
        counter++;
    }
}
