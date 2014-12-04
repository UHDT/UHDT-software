#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "pwm.h"
#include "globals.h"
#include "macros.h"
#include "util.h"

// TODO:
// Work on making the pwm functions more versatile
// Have it so that you can initialize any of the pwm channels
// Make it so that you can just say what frequency and period (within a range)


void pwm_init()
{
    pwm_timer_init();
    pwm_pins_init();
    pwm_channel_init();
}

//  TIM4 is connected to APB1 bus, which has on F407 device 42MHz clock
//  But, timer has internal PLL, which double this frequency for timer, up to 84MHz
//  Remember: Not each timer is connected to APB1, there are also timers connected
//  on APB2, which works at 84MHz by default, and internal PLL increase
//  this to up to 168MHz
//
//  Set timer prescaller
//  Timer count frequency is set with
//
//  timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)
//
//  In our case, we want a max frequency for timer, so we set prescaller to 0
//  And our timer will have tick frequency
//
//  timer_tick_frequency = 84000000 / (0 + 1) = 84000000

//  Set timer period when it have reset
//  First you have to know max value for timer
//  In our case it is 16bit = 65535
//  To get your frequency for PWM, queasion is simple
//
//  PWM_frequency = timer_tick_frequency / (TIM_Period + 1)
//
//  If you know your PWM frequency you want to have timer period set correct
//
//  TIM_Period = timer_tick_frequency / PWM_frequency - 1
//
//  In our case, for 10Khz PWM_frequency, set Period to
//
//  TIM_Period = 84000000 / 10000 - 1 = 8399
//
//  If you get TIM_Period larger than max timer value (in our case 65535),
//  you have to choose larger prescaller and slow down timer tick frequency

void pwm_timer_init() {
    TIM_TimeBaseInitTypeDef TIM_BaseStruct;

    /* Enable clock for TIM4 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_BaseStruct.TIM_Prescaler = 19; // prev 5
    /* Count up */
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStruct.TIM_Period = 65535; // prev 65535
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;
    /* Initialize TIM4 */
    TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);
    /* Start count on TIM4 */
    TIM_Cmd(TIM4, ENABLE);
}


//  Common settings
//
//  PWM mode 2 = Clear on compare match
//  PWM mode 1 = Set on compare match
//
//
//  To get proper duty cycle, you have simple equation
//
//  pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
//
//  where DutyCycle is in percent, between 0 and 100%
//
//  25% duty cycle:     pulse_length = ((8399 + 1) * 25) / 100 - 1 = 2099
//  50% duty cycle:     pulse_length = ((8399 + 1) * 50) / 100 - 1 = 4199
//  75% duty cycle:     pulse_length = ((8399 + 1) * 75) / 100 - 1 = 6299
//  100% duty cycle:    pulse_length = ((8399 + 1) * 100) / 100 - 1 = 8399
//
//  Remember: if pulse_length is larger than TIM_Period, you will have output HIGH all the time

void pwm_channel_init() {
    TIM_OCInitTypeDef TIM_OCStruct;
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

    TIM_OCStruct.TIM_Pulse = PULSE_ONE_MS; // 6.8% duty cycle
    TIM_OC1Init(TIM4, &TIM_OCStruct);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_OCStruct.TIM_Pulse = PULSE_ONE_MS; // 6.8% duty cycle
    TIM_OC2Init(TIM4, &TIM_OCStruct);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_OCStruct.TIM_Pulse = PULSE_ONE_MS; // 6.8% duty cycle
    TIM_OC3Init(TIM4, &TIM_OCStruct);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_OCStruct.TIM_Pulse = PULSE_ONE_MS; // 6.8% duty cycle
    TIM_OC4Init(TIM4, &TIM_OCStruct);
    TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);

}

void pwm_pins_init() {
    GPIO_InitTypeDef GPIO_InitStruct;

    // set clock for GPIOB
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_TIM4);

    // set pints for pwm
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void pwm_inc_to_percent(int *motor, int percent, MotorFunction motor_func)
{
    // pwm_value we want to go to
    int pwm_value = PULSE_ONE_MS + percent * PULSE_ONE_PERCENT;
    // how much to increment motors by
    int increment = pwm_value > *motor ? 1 : -1;
    int pwm_difference = abs(pwm_value - *motor);
    int i = 0;
    // increment motor values to certain given percentage
    for (i = 0; i < pwm_difference; ++i)
    {
        *motor += increment;
        motor_func(TIM4, *motor);
    }
}

void pwm_inc_by_percent(int *motor, int percent, MotorFunction motor_func)
{
    int pwm_value = percent*PULSE_ONE_PERCENT;
    int final_pwm = *motor + pwm_value;

    if (final_pwm > PULSE_ONE_MS*2) {
        pwm_value = PULSE_ONE_MS*2;
    } else if (final_pwm < PULSE_ONE_MS) {
        pwm_value = PULSE_ONE_MS;
    }
    int increment = percent < 0 ? -1 : 1;

    int i = 0;
    for (i = 0; i < pwm_value; ++i)
    {
        *motor += increment;
        motor_func(TIM4, *motor);
    }
}

void pwm_inc_to_value(int *motor, int pwm_value, MotorFunction motor_func)
{
    // how much to increment motors by
    int increment = pwm_value > *motor ? 1 : -1;
    int pwm_difference = abs(pwm_value - *motor);
    int i = 0;
    // increment motor values to certain given percentage
    for (i = 0; i < pwm_difference; ++i)
    {
        *motor += increment;
        motor_func(TIM4, *motor);
    }
}

void pwm_inc_by_value(int *motor, int pwm_value, MotorFunction motor_func)
{
    int final_pwm = *motor + pwm_value;

    if (final_pwm > PULSE_ONE_MS*2) {
        pwm_value = PULSE_ONE_MS*2;
    } else if (final_pwm < PULSE_ONE_MS) {
        pwm_value = PULSE_ONE_MS;
    }
    int increment = pwm_value < 0 ? -1 : 1;

    int i = 0;
    for (i = 0; i < pwm_value; ++i)
    {
        *motor += increment;
        motor_func(TIM4, *motor);
    }
}


