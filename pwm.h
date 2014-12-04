#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDE

typedef void (*MotorFunction)(TIM_TypeDef*, uint32_t);

void                                    pwm_init();
void                                    pwm_timer_init();
void                                    pwm_channel_init();
void                                    pwm_pins_init();

void                                    pwm_inc_to_percent(int *motor, int percent, MotorFunction motor_func);
void                                    pwm_inc_by_percent(int *motor, int percent, MotorFunction motor_func);
void                                    pwm_inc_to_value(int *motor, int value, MotorFunction motor_func);
void                                    pwm_inc_by_value(int *motor, int value, MotorFunction motor_func);

#endif /* PWM_H_INCLUDED */
