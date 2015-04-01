#ifndef DATA_QUEUE_H
#define DATA_QUEUE_H

#include <string.h>
#include "protocol.h"
#include "globals.h"

#define TRUE 1
#define FALSE 0
#define DATA_QUEUE_MAX 100

#ifdef MAIN
int g_dataqueue_size;
int g_dataqueue_front;
#else
extern int g_dataqueue_size;
extern int g_dataqueue_front;
#endif

typedef struct
{
    uint8_t datatype;
    uint8_t size;
    uint8_t data [DATA_PACKET_SIZE];
} DataQueue;

int dataqueue_add (DataQueue*,uint8_t,uint8_t,uint8_t*);
DataQueue * dataqueue_remove (DataQueue[]);
DataQueue * dataqueue_peek (DataQueue[]);

#endif
