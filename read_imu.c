#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_i2c.h"
#include "semihosting.h"
#include "spi_init.h"
#include "i2c3_init.h"
#include "read_imu.h"
#include <inttypes.h>

//Gyro Address: 1101011
//Acc Address: 0011001
//Mag Address: 0011110

void configure_gyro()
{
	I2C_start(I2C2, 0b1101011<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C2, 0x20); // write one byte to register 20 of gyroscope
	I2C_write(I2C2, 0b00111111); //gyro output at 95 Hz, enable x, y, z axes
	I2C_stop(I2C2); // stop the transmission

	I2C_start(I2C2, 0b1101011<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C2, 0x21); // write one byte to register 21 of gyroscope
	I2C_write(I2C2, 0b00101001); //normal high-pass configuration,
	I2C_stop(I2C2); // stop the transmission

	I2C_start(I2C2, 0b1101011<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C2, 0x23); // write one byte to register 23 of gyroscope
	I2C_write(I2C2, 0b00100000); //2000 dps
	I2C_stop(I2C2); // stop the transmission
}

void configure_accelerometer()
{
	I2C_start(I2C2, 0b0011001<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C2, 0x20); // write one byte to register 20 of accelerometer
	I2C_write(I2C2, 0b01110111); //Data output rate of 400 Hz, enable x, y, z axes
	I2C_stop(I2C2); // stop the transmission

	I2C_start(I2C2, 0b0011001<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C2, 0x21); // write one byte to register 21 of accelereometer
	I2C_write(I2C2, 0b00000000); //normal high-pass configuration
	I2C_stop(I2C2); // stop the transmission

	I2C_start(I2C2, 0b0011001<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C2, 0x23); // write one byte to register 23 of accelerometer
	I2C_write(I2C2, 0b00011000); //+/- 4G
	I2C_stop(I2C2); // stop the transmission
}

void configure_mag()
{
	I2C_start(I2C2, 0b0011110<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C2, 0x00); // write one byte to register 0 of magnetometer
	I2C_write(I2C2, 0b00010100); //Minimum data output rate of 30 Hz, temp sensor disabled
	I2C_stop(I2C2); // stop the transmission

	I2C_start(I2C2, 0b0011110<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C2, 0x01); // write one byte to register 1 of magnetometer
	I2C_write(I2C2, 0b00100000); //Set gain x and y to 1100 LSB/Gauss and z to 980 LSB/Gauss
	I2C_stop(I2C2); // stop the transmission

	I2C_start(I2C2, 0b0011110<<1, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C2, 0x02); // write one byte to register 2 of magnetometer
	I2C_write(I2C2, 0b00000000); //Continuous conversion mode
	I2C_stop(I2C2); // stop the transmission
}

float read_gyro(uint8_t reg_h,uint8_t reg_l)
{
	int16_t raw_gyro;
	float rate_gyro; //deg/s
	float ang_disp; //deg

	//Read Gyroscope MSBs
	I2C_start(I2C2, 0b1101011<<1, I2C_Direction_Transmitter); // start a transmission in Master receiver mode
	I2C_write(I2C2, reg_h);
	I2C_stop(I2C2);
	I2C_start(I2C2, 0b1101011<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode
	raw_gyro = I2C_read_nack(I2C2);

	//Read Gyroscope LSBs
	I2C_start(I2C2, 0b1101011<<1, I2C_Direction_Transmitter); // start a transmission in Master receiver mode
	I2C_write(I2C2, reg_l);
	I2C_stop(I2C2);
	I2C_start(I2C2, 0b1101011<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode
	//Concatenate MSBs and LSBs
	raw_gyro = ~((raw_gyro<<8) | I2C_read_nack(I2C2));

	//Convert gyroscope to true rate based on resolution
	rate_gyro = (float)raw_gyro*.07;
	//Return gyroscope rate
	return rate_gyro;

}

float read_accelerometer(uint8_t reg_h,uint8_t reg_l)
{
	int16_t raw_acc;
	float rate_acc; //m/s^2

	//Read Acc MSBs
	I2C_start(I2C2, 0b0011001<<1, I2C_Direction_Transmitter); // start a transmission in Master receiver mode
	I2C_write(I2C2, reg_h);
	I2C_stop(I2C2);
	I2C_start(I2C2, 0b0011001<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode
	raw_acc = I2C_read_nack(I2C2);
	//Read Acc LSBs
	I2C_start(I2C2, 0b0011001<<1, I2C_Direction_Transmitter); // start a transmission in Master receiver mode
	I2C_write(I2C2, reg_l);
	I2C_stop(I2C2);
	I2C_start(I2C2, 0b0011001<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode
	//Concatenate MSBs and LSBs
	raw_acc =  (int16_t)((raw_acc<<8) | I2C_read_nack(I2C2))>>4;

	//Convert raw accelerometer to true acceleration
	rate_acc = ((float)raw_acc)/1000*9.8;
	//return acceleration
	return rate_acc;
}

float read_mag(uint8_t reg_h, uint8_t reg_l)
{
	int16_t raw_mag;
	float position;

	//Read Mag MSBs
	I2C_start(I2C2, 0b0011110<<1, I2C_Direction_Transmitter); // start a transmission in Master receiver mode
	I2C_write(I2C2, reg_h);
	I2C_stop(I2C2);
	I2C_start(I2C2, 0b0011110<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode
	raw_mag = I2C_read_nack(I2C2);
	//Read Mag LSBs
	I2C_start(I2C2, 0b0011110<<1, I2C_Direction_Transmitter); // start a transmission in Master receiver mode
	I2C_write(I2C2, reg_l);
	I2C_stop(I2C2);
	I2C_start(I2C2, 0b0011110<<1, I2C_Direction_Receiver); // start a transmission in Master receiver mode
	//concatenate MSBs and LSBS
	raw_mag =  (int16_t)(raw_mag<<8) | I2C_read_nack(I2C2);

	//Convert X and Y magnetometer readings
	if(reg_h == 0x03 || reg_h == 0x07) position = ((float)raw_mag);///1100);
	//Convert Z magnetometer readings
	else position = (float)raw_mag;///980;

	//Return magnetometer readings
	return position;
}
