#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "pwm.h"
#include "macros.h"
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
    Motor                           g_right_motor = { .PULSE_VALUE = RIGHT_MOTOR_PULSE, .MOTOR_FUNC = &RIGHT_MOTOR_FUNC };
    Motor                           g_left_motor = { .PULSE_VALUE = LEFT_MOTOR_PULSE, .MOTOR_FUNC = &LEFT_MOTOR_FUNC };
    Motor                           g_front_motor = { .PULSE_VALUE = FRONT_MOTOR_PULSE, .MOTOR_FUNC = &FRONT_MOTOR_FUNC };
    Motor                           g_back_motor = { .PULSE_VALUE = BACK_MOTOR_PULSE, .MOTOR_FUNC = &BACK_MOTOR_FUNC };

#else
//    extern int g_pwm_value;
    extern Mag                      g_mag;
    extern Accel                    g_accel;
    extern Gyro                     g_gyro;
    extern Angle                    g_ang;
    extern int                      g_roll_setpoint;
    extern Motor                    g_right_motor;
    extern Motor                    g_left_motor;
#endif
