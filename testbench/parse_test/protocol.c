//#define TESTBENCH

#include "protocol.h"

#ifndef TESTBENCH
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
    uint8_t packet[DATA_PACKET_SIZE] = {0};

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
#endif

uint8_t * protocol_packet_generator (uint8_t packet [], DataQueue * queue)
{
    int index = 5;
    int size = 0;

    if (dataqueue_peek(queue) == NULL)
    {
        return NULL;
    }
    else
    {
        packet[0] = (SOURCE_ID >> 8) & 0xFF;
        packet[1] = SOURCE_ID & 0xFF;
        packet[2] = (DEST_ID >> 8) & 0xFF;
        packet[3] = DEST_ID & 0xFF;
        size = protocol_data_packet_generator(packet,&index,queue);
        packet[4] = size;
        packet[size-1] = PACKET_END;

#ifndef TESTBENCH
        //tx_request(packet,size);
#endif
    }
    g_count++;
    return packet;
}

int protocol_data_packet_generator (uint8_t packet[],int * index, DataQueue * queue)
{
    int size = (*index);
    int count = 0;
    int debug = 0;
    DataQueue * temp = dataqueue_peek(queue);

    while (temp != NULL && g_dataqueue_size > 0)
    {
        if ((temp->size+size+1) >= (PACKET_SIZE-6))
        {
            break;
        }
        temp = dataqueue_remove(queue);
        size = size + temp->size + 1;
        packet[(*index)++] = temp->datatype;
        for (count = 0; count < temp->size; count++)
        {
            packet[(*index)++] = temp->data[count];
        }

        temp = dataqueue_peek(queue);
    }

    return size+1;
}

int protocol_packet_parser()
{
    int index = 15;
    int size = 0;
    int count = 0;
    int datasize = 0;

    // index+0 and index+1 are the source id of the transmitter
    // Currently there are only two endpoints but eventually
    // multiple DEST_ID's will need to be checked against
    // these values.
    if((Rx_Buffer[index+0]<<8|Rx_Buffer[index+1]) != DEST_ID)
        return FALSE;

    // index+2 and index+3 are the id of the intended receiver.
    // If this end was supposed to receive this packet, the 
    // value should match this machines SOURCE_ID value.
    if((Rx_Buffer[index+2]<<8|Rx_Buffer[index+3]) != SOURCE_ID)
        return FALSE;

    // index+4 contains the overall size of the packet transmitted
    size = Rx_Buffer[index+4];

    // Parses the data in the packet
    for (index = index+5; index < size-1; index++)
    {
        datasize = parse_data(Rx_Buffer[index],index+1);

        // Either there was an error in the packet or a false
        // datatype was given. Either way this packet should
        // be ignored.
        if (datasize == 0)
            return FALSE;

        index += datasize;
    }

    if(Rx_Buffer[index+(size-1)] != PACKET_END)
        return FALSE;

    return TRUE;
}
