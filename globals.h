#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "imu.h"
void global_init();

#endif /* GLOBALS_H_INCLUDED */


#ifdef MAIN_FILE
//    int g_pwm_value;
    Mag                             g_mag;
    Accel                           g_accel;
    Gyro                            g_gyro;
    Angle                           g_ang;
    int                             g_roll_setpoint;
    // used for roll (x-axis)
    int                             g_right_motor;
    int                             g_left_motor;
#else
//    extern int g_pwm_value;
    extern Mag                      g_mag;
    extern Accel                    g_accel;
    extern Gyro                     g_gyro;
    extern Angle                    g_ang;
    extern int                      g_roll_setpoint;
    extern int                      g_right_motor;
    extern int                      g_left_motor;
#endif
