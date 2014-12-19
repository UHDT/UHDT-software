#include <stdio.h>
#include <inttypes.h>
#include <math.h>
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_i2c.h"
#include "i2c.h"
#include "imu.h"
#include "macros.h"

//Gyro Address: 1101011
//Acc Address: 0011001
//Mag Address: 0011110

// Configure the imu's gyro to the right settings by sending it values via I2C.
// @param: none
// @return: none
void imu_configure_gyro()
{
	i2c_start(I2C2, IMU_GYRO_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	i2c_write(I2C2, 0x20); // write one byte to register 20 of gyroscope
	i2c_write(I2C2, 0b00111111); //gyro output at 95 Hz, enable x, y, z axes
	i2c_stop(I2C2); // stop the transmission

	i2c_start(I2C2, IMU_GYRO_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	i2c_write(I2C2, 0x21); // write one byte to register 21 of gyroscope
	i2c_write(I2C2, 0b00101001); //normal high-pass configuration,
	i2c_stop(I2C2); // stop the transmission

	i2c_start(I2C2, IMU_GYRO_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	i2c_write(I2C2, 0x23); // write one byte to register 23 of gyroscope
	i2c_write(I2C2, 0b00100000); //2000 dps
	i2c_stop(I2C2); // stop the transmission
}

// Configure the imu's accelerometer to the right settings by sending it values via I2C.
// @param: none
// @return: none
void imu_configure_accelerometer()
{
	i2c_start(I2C2, IMU_ACCEL_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	i2c_write(I2C2, 0x20); // write one byte to register 20 of accelerometer
	i2c_write(I2C2, 0b01110111); //Data output rate of 400 Hz, enable x, y, z axes
	i2c_stop(I2C2); // stop the transmission

	i2c_start(I2C2, IMU_ACCEL_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	i2c_write(I2C2, 0x21); // write one byte to register 21 of accelereometer
	i2c_write(I2C2, 0b00000000); //normal high-pass configuration
	i2c_stop(I2C2); // stop the transmission

	i2c_start(I2C2, IMU_ACCEL_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	i2c_write(I2C2, 0x23); // write one byte to register 23 of accelerometer
	i2c_write(I2C2, 0b00011000); //+/- 4G
	i2c_stop(I2C2); // stop the transmission
}


// Configure the imu's magnetometer to the right settings by sending it values via I2C.
// @param: none
// @return: none
void imu_configure_mag()
{
	i2c_start(I2C2, IMU_GYRO_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	i2c_write(I2C2, 0x00); // write one byte to register 0 of magnetometer
	i2c_write(I2C2, 0b00010100); //Minimum data output rate of 30 Hz, temp sensor disabled
	i2c_stop(I2C2); // stop the transmission

	i2c_start(I2C2, IMU_GYRO_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	i2c_write(I2C2, 0x01); // write one byte to register 1 of magnetometer
	i2c_write(I2C2, 0b00100000); //Set gain x and y to 1100 LSB/Gauss and z to 980 LSB/Gauss
	i2c_stop(I2C2); // stop the transmission

	i2c_start(I2C2, IMU_GYRO_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	i2c_write(I2C2, 0x02); // write one byte to register 2 of magnetometer
	i2c_write(I2C2, 0b00000000); //Continuous conversion mode
	i2c_stop(I2C2); // stop the transmission
}

// Read the gyro data from the IMU
// @param: reg_h - gyro's MSB
// @param: reg_l - gyro's LSB
// @return: the gyro data
float imu_read_gyro(uint8_t reg_h,uint8_t reg_l)
{
	int16_t raw_gyro;
	float rate_gyro; //deg/s

	//Read Gyroscope MSBs
	i2c_start(I2C2, IMU_GYRO_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master receiver mode
	i2c_write(I2C2, reg_h);
	i2c_stop(I2C2);
	i2c_start(I2C2, IMU_GYRO_ADDR<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode
	raw_gyro = i2c_read_nack(I2C2);

	//Read Gyroscope LSBs
	i2c_start(I2C2, IMU_GYRO_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master receiver mode
	i2c_write(I2C2, reg_l);
	i2c_stop(I2C2);
	i2c_start(I2C2, IMU_GYRO_ADDR<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode
	//Concatenate MSBs and LSBs
	raw_gyro = ~((raw_gyro<<8) | i2c_read_nack(I2C2));

	//Convert gyroscope to true rate based on resolution
	rate_gyro = (float)raw_gyro*.07;
	//Return gyroscope rate
	return rate_gyro;

}

// Read the accelerometer data from the IMU
// @param: reg_h - accels's MSB
// @param: reg_l - accels's LSB
// @return: the accel data
float imu_read_accelerometer(uint8_t reg_h, uint8_t reg_l)
{
	int16_t raw_acc;
	float rate_acc; //m/s^2

	//Read Acc MSBs
	i2c_start(I2C2, IMU_ACCEL_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master receiver mode
	i2c_write(I2C2, reg_h);
	i2c_stop(I2C2);
	i2c_start(I2C2, IMU_ACCEL_ADDR<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode
	raw_acc = i2c_read_nack(I2C2);
	//Read Acc LSBs
	i2c_start(I2C2, IMU_ACCEL_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master receiver mode
	i2c_write(I2C2, reg_l);
	i2c_stop(I2C2);
	i2c_start(I2C2, IMU_ACCEL_ADDR<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode
	//Concatenate MSBs and LSBs
	raw_acc =  (int16_t)((raw_acc<<8) | i2c_read_nack(I2C2))>>4;

	//Convert raw accelerometer to true acceleration
	rate_acc = ((float)raw_acc)/1000*9.8;
	//return acceleration
	return rate_acc;
}

// Read the magnetometer data from the IMU
// @param: reg_h - mags's MSB
// @param: reg_l - mags's LSB
// @return: the mag data
float imu_read_mag(uint8_t reg_h, uint8_t reg_l)
{
	int16_t raw_mag;
	float position;

	//Read Mag MSBs
	i2c_start(I2C2, IMU_MAG_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master receiver mode
	i2c_write(I2C2, reg_h);
	i2c_stop(I2C2);
	i2c_start(I2C2, IMU_MAG_ADDR<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode
	raw_mag = i2c_read_nack(I2C2);
	//Read Mag LSBs
	i2c_start(I2C2, IMU_MAG_ADDR<<1, I2C_Direction_Transmitter); // start a transmission in Master receiver mode
	i2c_write(I2C2, reg_l);
	i2c_stop(I2C2);
	i2c_start(I2C2, IMU_MAG_ADDR<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode
	//concatenate MSBs and LSBS
	raw_mag =  (int16_t)(raw_mag<<8) | i2c_read_nack(I2C2);

	//Convert X and Y magnetometer readings
	if(reg_h == 0x03 || reg_h == 0x07) position = ((float)raw_mag);///1100);
	//Convert Z magnetometer readings
	else position = (float)raw_mag;///980;

	//Return magnetometer readings
	return position;
}

// Reads gyro data from the IMU, converts it to usable data, then stores
// it.
// @param: gyro - where the usable data will be stored
// @return: none
void imu_fill_gyro_data(Gyro *gyro)
{
    float dt = .085;
    //Read gyro x, y, z axes
    gyro->x = imu_read_gyro(0x29,0x28);
    gyro->y = imu_read_gyro(0x2B,0x2A);
    gyro->z = imu_read_gyro(0x2D,0x2C);

    //Calculate angles with respect to axes using gyroscope
    gyro->x_ang += gyro->x*dt;
    gyro->y_ang += gyro->y*dt;
    gyro->z_ang += gyro->z*dt;
}

// Reads magnetometer data from the IMU, converts it to usable data, then stores
// it.
// @param: mag - where the usable data will be stored
// @return: none
void imu_fill_mag_data(Mag *mag)
{
    //Read magnetometer x, y, z axes
    mag->x = imu_read_mag(0x03,0x04);
    mag->z = imu_read_mag(0x05,0x06);
    mag->y = imu_read_mag(0x07,0x08);

}

// Reads accelerometer data from the IMU, converts it to usable data, then stores
// it.
// @param: acc - where the usable data will be stored
// @return: none
void imu_fill_accel_data(Accel *acc)
{
    //Read accelerometer x, y, z axes
    acc->x = GAIN*imu_read_accelerometer(0x29,0x28);
    acc->y = GAIN*imu_read_accelerometer(0x2B,0x2A);
    acc->z = GAIN*imu_read_accelerometer(0x2D,0x2C);

            //Calculate Angles with respect to axes using accelerometer
    acc->y_ang = (180/PI*atan2f((float)acc->x,(sqrtf(acc->y*acc->y+acc->z*acc->z))));
    acc->x_ang = (180/PI*atan2f((float)acc->y,(sqrtf(acc->x*acc->x+acc->z*acc->z))));
    acc->z_ang = (180/PI*atan2f((sqrtf(acc->x*acc->x+acc->y*acc->y)),(float)acc->z));
}

// Takes the data from the gyro, accelerometer, and magnetometer and converts it
// into usable angle data
// @param: ang - where all the data will be stored
// @param: gyro - contains all the gyro data used in calculations
// @param: mag - contains all the mag data used in calculations
// @param: acc - contains all the accel data used in calculations
void imu_fill_angle_data(Angle *ang, Gyro *gyro, Mag *mag, Accel *acc)
{
    float xh, yh;
    float dt = .085;

    //Complementary Filter
    ang->comp_x = (COMP_GAIN*(float)(ang->comp_x + gyro->x*dt) + ((1-COMP_GAIN)*(float)acc->x_ang));
    ang->comp_y = (COMP_GAIN*(float)(ang->comp_y + gyro->y*dt) + ((1-COMP_GAIN)*(float)acc->y_ang));

    //Calculate Yaw angle
    xh = mag->x*cosf((float)ang->comp_y) + mag->y*sinf((float)ang->comp_x) * sinf((float)ang->comp_y) +
         mag->z*cosf((float)ang->comp_x)*sinf((float)ang->comp_y);
    yh = -mag->y*cosf((float)ang->comp_x) + mag->z*sinf((float)ang->comp_x);
    ang->yaw = (180/PI)*atan2f(yh, xh);
    ang->comp_z = COMP_GAIN*(float)(ang->comp_z - gyro->z*dt) + ((1-COMP_GAIN)*(float)ang->yaw);

}



