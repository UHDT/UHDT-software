#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include "pwm.h"
#include "macros.h"
#include "imu.h"

void global_init();

#endif /* GLOBALS_H_INCLUDED */

// In order to use this, you must put #define MAIN_FILE before #include "globals.h"
// in exactly ONE file. Then in all other files, you just put #include "globals.h"
// This will define the globals once (in the MAIN_FILE) and make a reference to
// that global in all the others.

#ifdef MAIN_FILE
    Mag                             g_mag;
    Accel                           g_accel;
    Gyro                            g_gyro;
    Angle                           g_ang;

    // this is the setpoint for the roll. If you change this value, you can tilt more
    // in one direction than another.
    int                             g_roll_setpoint;

    // all the different motors
    Motor                           g_right_motor = { .PULSE_VALUE = RIGHT_MOTOR_PULSE, .MOTOR_FUNC = &RIGHT_MOTOR_FUNC };
    Motor                           g_left_motor = { .PULSE_VALUE = LEFT_MOTOR_PULSE, .MOTOR_FUNC = &LEFT_MOTOR_FUNC };
    Motor                           g_front_motor = { .PULSE_VALUE = FRONT_MOTOR_PULSE, .MOTOR_FUNC = &FRONT_MOTOR_FUNC };
    Motor                           g_back_motor = { .PULSE_VALUE = BACK_MOTOR_PULSE, .MOTOR_FUNC = &BACK_MOTOR_FUNC };

#else
    extern Mag                      g_mag;
    extern Accel                    g_accel;
    extern Gyro                     g_gyro;
    extern Angle                    g_ang;
    extern int                      g_roll_setpoint;
    extern Motor                    g_right_motor;
    extern Motor                    g_left_motor;
    extern Motor                    g_back_motor;
    extern Motor                    g_front_motor;
#endif
