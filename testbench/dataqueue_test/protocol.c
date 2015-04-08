#define TESTBENCH

#include "protocol.h"

/*
void protocol_init_data ()
{
    g_data.latitude = 0;
    g_data.longitude = 0;
    g_data.altitude = 0;
    g_data.velocity = 0;
    g_data.orientation = 0;
    g_data.power = 0;
    g_data.signal_strength = 0;
    g_data.dest_lat = 0;
    g_data.dest_lon = 0;
}

void protocol_tx_data ()
{
    uint8_t packet[DATA_PACKET_SIZE];

    memcpy(packet,&g_data,DATA_PACKET_SIZE);
    tx_request(packet,DATA_PACKET_SIZE);
}

void protocol_rx_data ()
{
    int index = 15;
    g_data.latitude = Rx_Buffer[index++];
    g_data.longitude = Rx_Buffer[index++];
    g_data.altitude = Rx_Buffer[index++];
    g_data.velocity = Rx_Buffer[index++];
    g_data.orientation = Rx_Buffer[index++];
    g_data.power = Rx_Buffer[index++];
    g_data.signal_strength = Rx_Buffer[index++];
    g_data.dest_lat = Rx_Buffer[index++];
    g_data.dest_lon = Rx_Buffer[index++];
}
*/

#ifndef TESTBENCH
void protocol_packet_generator ()
{
    uint8_t packet[DATA_PACKET_SIZE] = {0};

    packet[0] = (SOURCE_ID >> 8) & 0xFF;
    packet[1] = SOURCE_ID & 0xFF;
    packet[2] = (DEST_ID >> 8) & 0xFF;
    packet[3] = DEST_ID & 0xFF;
    packet[4] = DATA_PACKET_SIZE;
    packet[DATA_PACKET_SIZE-1] = PACKET_END;

    tx_request(packet,DATA_PACKET_SIZE);
}
#endif

void protocol_data_packet_generator (uint8_t packet[])
{
}

void protocol_debug ()
{
    printf("%d ",g_data.latitude);
    printf("%d ",g_data.longitude);
    printf("%d ",g_data.altitude);
    printf("%d ",g_data.velocity);
    printf("%d ",g_data.orientation);
    printf("%d ",g_data.power);
    printf("%d ",g_data.signal_strength);
    printf("%d ",g_data.dest_lat);
    printf("%d ",g_data.dest_lon);
}