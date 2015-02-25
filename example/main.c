/*
	This example will continue to send a packet
	after a given delay.  It will also receive any
	incoming packet and execute whatever
	is included in the Rx function.  (currently echoes
	what was received)

	Make sure to add .h files found in xbee.h into your
	source folder.

*/

#define MAIN
#include "globals.h"
#include "xbee.h"
#include "protocol.h"

void Delay(__IO uint32_t nCount);

uint8_t escaped = FALSE;

int main(void)
{
	init_USART1(9600);

	//DroneData data;
	/*
	uint8_t stuffs[10];
	uint8_t i;

	for ( i = 0; i < 10; i++ )
        stuffs[i] = i+1;

	stuffs[3] = 0x7D;
	*/

	protocol_init_data();

	while(1)
	{
		//tx_request(stuffs, sizeof(stuffs));
		//protocol_tx_data();
		protocol_tx_data();
		Delay(0xFFFFFF);
	}
}

void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}
