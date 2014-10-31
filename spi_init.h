#include <stdint.h>
void mySPI_Init(void);
void mySPI_SendData(uint8_t adress, uint8_t data);
uint8_t mySPI_GetData(uint8_t adress);
