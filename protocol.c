#include "protocol.h"
#include "globals.h"
#include "xbee.h"

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
	uint8_t packet[PACKET_DATA_SIZE];

	memcpy(packet,&g_data,PACKET_DATA_SIZE);
	tx_request(packet,PACKET_DATA_SIZE);
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
