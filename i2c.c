#include <stdio.h>
#include <inttypes.h>
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_i2c.h"

//slave address of gyro = 0b1101011
//slave address of accelerometer = 0b0011001
//slave address of magnetometer = 0b0011110

// Initialize the I2C clockspeed, mode, duty, etc. Initializes the
// pins that are needed for the I2C. Initializes the clocks as well.
// @param: none
// @return: none
void i2c_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	I2C_InitTypeDef I2C_InitStruct;

	// enable APB1 peripheral clock for I2C2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
	// enable clock for SCL and SDA pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	// setup SCL and SDA pins
	// 1. SCL on PB10
	// 2. SDA on PB11
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;     // we are going to use PB10 and PB11
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			            // set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;	          // set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			          // set output to open drain --> the line has to be only
                                                            //                              pulled low, not driven high
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			            // enable pull up resistors
	GPIO_Init(GPIOB, &GPIO_InitStruct);					              // init GPIOB

	// Connect I2C1 pins to AF
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);	// SCL
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);  // SDA

	// configure I2C1
	I2C_InitStruct.I2C_ClockSpeed = 100000; 		              // 100kHz
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;			              // I2C mode
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;	          // 50% duty cycle --> standard
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;			              // own address, not relevant in master mode
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;		              // disable acknowledge when reading (can be changed later on)
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
	I2C_Init(I2C2, &I2C_InitStruct);				                  // init I2C1

	// enable I2C1
	I2C_Cmd(I2C2, ENABLE);
}


// Start the I2C transfer by sending the slave a start signal as well
// as an address.
// @param: I2Cx - which I2C you are going to be using
// @param: address - the address of the slave
// @param: direction - specify if it is a transmit or a receive
// @return: none
void i2c_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction)
{
	// wait until I2C1 is not busy any more
	while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSY));

	// Send I2C1 START condition
	I2C_GenerateSTART(I2Cx, ENABLE);

	// wait for I2C1 Slave to acknowledge start condition
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_MODE_SELECT));

	// Send slave Address for
	I2C_Send7bitAddress(I2Cx, address, direction);

	// wait for I2Cx EV6, check if
	// either Slave has acknowledged Master transmitter or
	// Master receiver mode, depending on the transmission
	// direction
	if(direction == I2C_Direction_Transmitter){
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	}
	else if(direction == I2C_Direction_Receiver){
		while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	}
}

// Write data to a slave
// @param: I2Cx - which I2C we are referring to
// @param: data - the data that we want to write
// @return: none
void i2c_write(I2C_TypeDef* I2Cx, uint8_t data)
{
	I2C_SendData(I2Cx, data);
	// wait for I2C1 EV8 --> last byte is still being transmitted (last byte in SR, buffer empty), next byte can already be written
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING));
}


// Reads one byte from the slave device and acknowledges the byte (requests another byte)
// @param: I2Cx - which I2C we are referring to
// @return: none
uint8_t i2c_read_ack(I2C_TypeDef* I2Cx){
	// enable acknowledge of received data
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	// wait until one byte has been received
	while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	// read data from I2C data register and return data byte
	uint8_t data = I2C_ReceiveData(I2Cx);
	return data;
}

// Reads one byte from the slave, but doesn't care about acknowledging
// @param: I2Cx - which I2C we are referring to
// @return: none
uint8_t i2c_read_nack(I2C_TypeDef* I2Cx)
{
	// disable acknowledge of received data
	// nack also generates stop condition after last byte received
	// see reference manual for more info
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_GenerateSTOP(I2Cx, ENABLE);
	// wait until one byte has been received
	while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) );
	// read data from I2C data register and return data byte
	uint8_t data = I2C_ReceiveData(I2Cx);
	return data;
}

// Sends a signal telling the I2C that we are done
// @param: I2Cx - which I2C we are referring to
// @return: none
void i2c_stop(I2C_TypeDef* I2Cx)
{
	// Send I2C1 STOP Condition after last byte has been transmitted
	I2C_GenerateSTOP(I2Cx, ENABLE);
	// wait for I2C1 EV8_2 --> byte has been transmitted
	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
}


