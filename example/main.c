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
//#include "globals.h"
#include "xbee.h"
#include "protocol.h"
#include "dataqueue.h"
#include "interrupts.h"
#include "parsers.h"
#include "waypoints.h"

void Delay(__IO uint32_t nCount);

uint8_t escaped = FALSE;

int main(void)
{
    init_USART1(115200);
    int_init();
    init_waypoints();
    dataqueue_init();

    uint8_t stuffs[DATA_PACKET_SIZE] = {0};
    int count = 0;
    g_count = 0;

    uint8_t packet [10] = {1,2,3,4,5,6,7,8,9,0};
    uint8_t packet2 [3] = {0xFF,0xFE,0xEF};
    g_transmit_time = 1000;
    while(1)
    {
        //tx_request(stuffs, sizeof(stuffs));
        //send_byte(0xCC,0);
        //protocol_tx_data();
        dataqueue_add(g_queue,GPS,sizeof(packet),packet);
        //protocol_packet_generator(stuffs,g_queue);

        //tx_request(packet,sizeof(packet));
        printf(g_waypoints[0]);
        Delay(100000);
    }
}

void Delay(__IO uint32_t nCount)
{
    while(nCount--)
    {
    }
}
