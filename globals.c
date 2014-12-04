#include "globals.h"
#include "macros.h"

void global_init()
{
    g_gyro.x_ang = 0;
    g_gyro.y_ang = 0;
    g_gyro.z_ang = 0;

    g_roll_setpoint = 0;
    g_right_motor = PULSE_ONE_MS;
    g_left_motor = PULSE_ONE_MS;
}
