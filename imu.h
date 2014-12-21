#ifndef IMU_H_INCLUDED
#define IMU_H_INCLUDED

#include <stdint.h>

// Accelerometer data (readings should go here)
typedef struct Accel
{
    int x, y, z;
    float x_ang, y_ang, z_ang;
} Accel;

// Gyrometer data (readings should go here)
typedef struct Gyro
{
    float x, y, z;
    float x_ang, y_ang, z_ang;
} Gyro;

// Magnetometer data (readings should go here)
typedef struct Mag
{
    float x, y, z, norm;
} Mag;

// Complimentary filter angles
typedef struct Angle
{
    int comp_x, comp_y, comp_z;
    float yaw;
} Angle;

void                        imu_configure_gyro();
void                        imu_configure_accelerometer();
void                        imu_configure_mag();
float                       imu_read_gyro(uint8_t reg_h, uint8_t reg_l);
float                       imu_read_accelerometer(uint8_t reg_h, uint8_t reg_l);
float                       imu_read_mag(uint8_t reg_h, uint8_t reg_l);

void                        imu_fill_gyro_data(Gyro *gyro);
void                        imu_fill_mag_data(Mag *mag);
void                        imu_fill_accel_data(Accel *acc);
void                        imu_fill_angle_data(Angle *ang, Gyro *gyro, Mag *mag, Accel *acc);

#endif /* IMU_H_INCLUDED */
