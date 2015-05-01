//#define TESTBENCH

#include "dataqueue.h"

/********************************************************************
 *  Function Name: dataqueue_add
 *
 *  Description: Adds the given data to the struct at the end of
 *               the queue
 *
 *  Parameters: queue (DataQueue []): The array list that
 *                  acts as the buffer for all the information that
 *                  needs to be transmitted.
 *
 *              datatype (uint8_t): The type of data that is being queued
 *
 *              packetsize (uint8_t): The size of the data being queued
 *
 *              packet (uint8_t []): The data to be queued
 *
 *  Return Values: 0 (FALSE): Data was not added to the queue
 *
 *                 1 (TRUE): Data was added to the queue
 *
 *******************************************************************/
int dataqueue_add (DataQueue * queue, uint8_t datatype, uint8_t packetsize,uint8_t * packet)
{
    if (g_dataqueue_size >= DATA_QUEUE_MAX)
    {
        return FALSE;
    }
    if (packetsize > DATA_PACKET_SIZE)
    {
        return FALSE;
    }

    int index = ((g_dataqueue_front+g_dataqueue_size)%DATA_QUEUE_MAX);

    queue[index].datatype = datatype;
    queue[index].size = packetsize;
    memcpy(queue[index].data,packet,packetsize);
    g_dataqueue_size++;

    return TRUE;
}

/********************************************************************
 *  Function Name: dataqueue_remove
 *
 *  Description: Removes the struct at the front of the queue and returns it.
 *
 *  Parameters: queue (DataQueue []): The array list that
 *                  acts as the buffer for all the information that
 *                  needs to be transmitted.
 *
 *  Return Values: The struct that was at the front of the queue. If
 *                  the queue is empty, returns NULL
 *
 *******************************************************************/
DataQueue * dataqueue_remove (DataQueue * queue)
{
    if (g_dataqueue_size <= 0)
    {
        return NULL;
    }

    DataQueue * temp = &(queue[g_dataqueue_front]);

    g_dataqueue_front = (g_dataqueue_front + 1) % DATA_QUEUE_MAX;
    g_dataqueue_size--;

    return temp;
}

/********************************************************************
 *  Function Name: dataqueue_peek
 *
 *  Description: Returns the struct at the front of the queue
 *
 *  Parameters: queue (DataQueue []): The array list that
 *                  acts as the buffer for all the information that
 *                  needs to be transmitted.
 *
 *  Return Values: The struct at the front of the queue.
 *
 *******************************************************************/
DataQueue * dataqueue_peek (const DataQueue * queue)
{
    if (g_dataqueue_size <= 0)
    {
        return NULL;
    }

    return &(queue[g_dataqueue_front]);
}

void dataqueue_init ()
{
    g_dataqueue_size = 0;
    g_dataqueue_front = 0;
    g_dataqueue_lock = FALSE;
}
