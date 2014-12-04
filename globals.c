#include "globals.h"
#include "macros.h"

void global_init()
{
    g_gyro.x_ang = 0;
    g_gyro.y_ang = 0;
    g_gyro.z_ang = 0;

    g_roll_setpoint = 0;
    // initialize motors pwm values to the initialization pulse
    g_right_motor.pwm_value = g_right_motor.PULSE_VALUE;
    g_left_motor.pwm_value = g_left_motor.PULSE_VALUE;
    g_front_motor.pwm_value = g_front_motor.PULSE_VALUE;
    g_back_motor.pwm_value = g_back_motor.PULSE_VALUE;
}
