#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDE

void                                    pwm_init();
void                                    pwm_timer_init();
void                                    pwm_channel_init();
void                                    pwm_pins_init();

void                                    pwm_percent_motor(int *motor, int percent);
void                                    pwm_increment_motor(int *motor, int percent);

#endif /* PWM_H_INCLUDED */
