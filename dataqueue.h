#ifndef DATA_QUEUE_H
#define DATA_QUEUE_H

typedef struct
{
    uint8_t datatype;
    uint8_t size;
    uint8_t data [DATA_PACKET_SIZE];
    struct DataQueue * next;
} DataQueue;

int dataqueue_add (uint8_t type,uint8_t packetsize,uint8_t packet);
struct DataQueue * dataqueue_remove ();
struct DataQueue * dataqueue_peek ();
int dataqueue_delete();

#endif
