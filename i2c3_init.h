#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>
#include "stm32f4xx_i2c.h"
void I2C3_init(void);
void I2C_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void I2C_write(I2C_TypeDef* I2Cx, uint8_t data);
u8 I2C_read_ack(I2C_TypeDef* I2Cx);
u8 I2C_read_nack(I2C_TypeDef* I2Cx);
void I2C_stop(I2C_TypeDef* I2Cx);
