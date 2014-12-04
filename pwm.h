#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDED

#include <stdint.h>
#include "stm32f4xx_tim.h"

typedef void (*MotorFunction)(TIM_TypeDef*, uint32_t);
typedef struct
{
    int                     pwm_value;
    const int               PULSE_VALUE;
    const MotorFunction     MOTOR_FUNC;
} Motor;

void                        pwm_init();
void                        pwm_timer_init();
void                        pwm_channel_init();
void                        pwm_pins_init();
void                        pwm_cap_value(int *pwm_value, int min, int max);

void                        pwm_inc_to_value(Motor *motor, int value);
void                        pwm_inc_by_value(Motor *motor, int value);

/*
void                        pwm_inc_to_percent(int *motor, int percent, MotorFunction motor_func);
void                        pwm_inc_by_percent(int *motor, int percent, MotorFunction motor_func);
*/

#endif /* PWM_H_INCLUDED */
