#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED\

#include "imu.h"

#ifdef MAIN_FILE
//    int g_pwm_value;
    Mag g_mag;
    Accel g_accel;
    Gyro g_gyro;
    Angle g_ang;
#else
//    extern int g_pwm_value;
    extern Mag g_mag;
    extern Accel g_accel;
    extern Gyro g_gyro;
    extern Angle g_ang;
#endif

#endif /* GLOBALS_H_INCLUDED */
