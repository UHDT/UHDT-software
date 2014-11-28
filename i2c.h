#ifndef I2C_H_INCLUDED
#define I2C_H_INCLUDED

void                        i2c_init();
void                        i2c_start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void                        i2c_write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t                     i2c_read_ack(I2C_TypeDef* I2Cx);
uint8_t                     i2c_read_nack(I2C_TypeDef* I2Cx);
void                        i2c_stop(I2C_TypeDef* I2Cx);

#endif /* I2C_H_INCLUDED */
