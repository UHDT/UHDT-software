#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDE

#define PWM_ONE_MS                      4400
#define PWM_ONE_PERCENT                 45
#define PWM_INCREMENT                   1
#define PWM_DECREMENT                   -1

void pwm_timer_init();
void pwm_init();
void pwm_leds_init();
void pwm_run_motor(int percent, int delay_ms);
void pwm_increment_motor(int increment, int percent, int delay_ms);

#endif /* PWM_H_INCLUDED */
