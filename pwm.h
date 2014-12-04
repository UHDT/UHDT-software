#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDE

typedef void (*MotorFunction)(TIM_TypeDef*, uint32_t);

void                                    pwm_init();
void                                    pwm_timer_init();
void                                    pwm_channel_init();
void                                    pwm_pins_init();

void                                    pwm_percent_motor(int *motor, int percent, MotorFunction motor_func);
void                                    pwm_increment_motor(int *motor, int percent, MotorFunction motor_func);
void                                    pwm_value_motor(int *motor, int value, MotorFunction motor_func);

#endif /* PWM_H_INCLUDED */
