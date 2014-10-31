#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_i2c.h"
#include "stm32f4xx_tim.h"
#include "core_cm4.h"
#include "semihosting.h"
#include "macros.h"
#include "misc.h"
#include "system_stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "pwm.h"
#include "util.h"
#define MAIN_FILE
#include "globals.h"

float Q_angle = .05;//.003;// increasing helps angle accuracy
float Q_gyro = .001;//.07;// increasing helps overshoot
float R_angle = .1;//.1;// increasing helps vibration
int count = 0;
float bias = 0;
float P[2][2] = {0};
float angle = 0;
float err_tot = 0;
float err_prev = 0;
int main(void)
{

	/* Set unbuffered mode for stdout (newlib) */
	//setvbuf( stdout, 0, _IONBF, 0 );
    /* Initialize system */
    SystemInit();
    /* Init leds */
    pwm_timer_init( );
    /* Init timer */
    pwm_init();
    /* Init PWM */
    pwm_leds_init();
  	I2C1_init(); // initialize I2C peripheral
	gpio_init();
	configure_gyro();
	configure_accelerometer();
	configure_mag();
	timer_init();
	EnableTimerInterrupt();
	NVIC_EnableIRQ(TIM2_IRQn);
	g_pwm_value = PWM_ONE_MS;
    util_delay_ms(1000*5);
    int count = 0;
    int increment = PWM_INCREMENT;
    while (1) {
        pwm_increment_motor(increment, 10, 10);
        util_delay_ms(1000*5);
        printf("%d\n", g_pwm_value);
        count++;
        if (count % 3 == 0) {
            if (increment == PWM_INCREMENT) {
                increment = PWM_DECREMENT;
            } else {
                increment = PWM_INCREMENT;
            }
        }

    }

}
