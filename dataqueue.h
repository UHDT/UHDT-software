#ifndef DATA_QUEUE_H
#define DATA_QUEUE_H

#include <string.h>
#include <stdint.h>

#include "macros.h"

typedef struct
{
    uint8_t datatype;
    uint8_t size;
    uint8_t data [DATA_PACKET_SIZE];
} DataQueue;

#ifdef MAIN
volatile int g_dataqueue_size;
volatile int g_dataqueue_front;
int g_dataqueue_lock;
DataQueue g_queue[DATA_QUEUE_MAX];
#else
extern volatile int g_dataqueue_size;
extern volatile int g_dataqueue_front;
extern int g_dataqueue_lock;
extern DataQueue g_queue[DATA_QUEUE_MAX];
#endif

int dataqueue_add (DataQueue*,uint8_t,uint8_t,uint8_t*);
DataQueue * dataqueue_remove (DataQueue[]);
DataQueue * dataqueue_peek (const DataQueue[]);
void dataqueue_init ();

#endif
