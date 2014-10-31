#ifndef PWM_H_INCLUDED
#define PWM_H_INCLUDE

#define PWM_ONE_MS                      4400
#define PWM_ONE_PERCENT                 45
#define PWM_INCREMENT                   1
#define PWM_DECREMENT                   -1

void pwm_init();
static void pwm_timer_init();
static void pwm_channel_init();
static void pwm_pins_init();

void pwm_percent_motor(int *motor, int percent);
void pwm_increment_motor(int *motor, int increment, int percent);

#endif /* PWM_H_INCLUDED */
