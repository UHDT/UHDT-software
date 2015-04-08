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
#include "macros.h"
#include "globals.h"
#include "xbee.h"
#include "protocol.h"
#include "dataqueue.h"

void Delay(__IO uint32_t nCount);

uint8_t escaped = FALSE;

int main(void)
{
    init_USART1(9600);
    //uint8_t stuffs[DATA_PACKET_SIZE] = {0};
    uint8_t stuffs[10] = {0};
    DataQueue queue[DATA_QUEUE_MAX] = {0};
    int count = 0;
    dataqueue_init();


    for (count = 0; count < sizeof(stuffs); count++)
    {
        stuffs[count] = count;
    }

    protocol_init_data();


    while(1)
    {
        tx_request(stuffs, sizeof(stuffs));
    	//send_byte(0xCC,0);
        //protocol_tx_data();
        //protocol_packet_generator(stuffs,queue);
        Delay(0xFFFFFF);
    }
}

void Delay(__IO uint32_t nCount)
{
    while(nCount--)
    {
    }
}
