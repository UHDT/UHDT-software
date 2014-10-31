#include <stdio.h>
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "semihosting.h"
#include "stm32f4xx_i2c.h"
#include <inttypes.h>

void configure_gyro();
void configure_accelerometer();
float read_gyro(uint8_t, uint8_t);
float read_accelerometer(uint8_t, uint8_t);
float read_mag(uint8_t, uint8_t);
