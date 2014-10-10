#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDE

#define PWM_ONE_MS 4400
#define PWM_ONE_PERCENT 45

void pwm_timer_init( );
void pwm_init();
void pwm_leds_init();
void pwm_run_motor(int percent);


#endif /* PWM_H_INCLUDED */
