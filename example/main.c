/*
	This example will continue to send a packet
	after a given delay.  It will also receive any
	incoming packet and execute whatever
	is included in the Rx function.  (currently echoes
	what was received)

	Make sure to add .h files found in xbee.h into your
	source folder.

*/

#include "xbee.h"

void Delay(__IO uint32_t nCount);

int rx_count = 0;

int main(void)
{
	init_USART1(9600);

	uint8_t stuffs[10];
	uint8_t i;

	for ( i = 0; i < 10; i++ )
        stuffs[i] = i+1;

	while(1)
	{
		tx_request(stuffs, sizeof(stuffs));
		Delay(0xFFFFFF);

		printf("%d\n",rx_count);
		/**
		if (rx_flag)
		{
			int count = 0;
			for (count = 0; count < 110; count++)
			{
				printf("%d",Rx_Buffer[count]);
			}
		}
		**/
	}
}





void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
