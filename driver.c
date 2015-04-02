#define TESTBENCH
#define MAIN

#include <stdio.h>
#include <string.h>

#include "globals.h"
#include "protocol.h"
#include "dataqueue.h"

void test_add (DataQueue*,uint8_t,uint8_t,uint8_t*,int*,int);
void test_remove (DataQueue*,int*,int);
void test_peek (DataQueue*,int*,int);
void test_packetgen (DataQueue*,int*,int);
void display_queue (DataQueue*);

int verbose = FALSE;

int main (int argc, char * argv[])
{
    if (argc == 2 && strcmp(argv[1],"verbose") == 0)
    {
        verbose = TRUE;
    }

    dataqueue_init();

    int testnum = 0;
    DataQueue queue[DATA_QUEUE_MAX];

    test_remove(queue,&testnum,0);

    uint8_t packet[80] = {0};
    test_add(queue,0xFF,80,packet,&testnum,1);

    uint8_t packet1[4] = {1,2,3,4};
    test_add(queue,0xFA,4,packet1,&testnum,2);

    test_packetgen(queue,&testnum,92);

    test_peek(queue,&testnum,0);

    test_remove(queue,&testnum,0);

    test_peek(queue,&testnum,0);

    if (verbose)
    {
        display_queue(queue);
    }
}

void test_add (DataQueue * queue, uint8_t datatype, uint8_t packetsize, uint8_t * packet, int * testnum, int expected)
{
    int count = 0;

    printf("\nTest %d: ADD\n",(*testnum));

    if (verbose)
    {
        printf("Parameters:\n");
        printf("\tqueue: 0x%X\n",queue);
        printf("\tdatatype: 0x%X\n",datatype);
        printf("\tpacketsize: %d\n",packetsize);
        printf("\tpacket:\n");
        for (count = 0; count < packetsize; count++)
        {
            printf("\t\t%d\n",packet[count]);
        }
        printf("\texpected: %d\n",expected);
    }
    dataqueue_add(queue,datatype,packetsize,packet);
    printf("Test %d Result: %s\n",(*testnum)++,(g_dataqueue_size==expected ? "PASS" : "FAIL"));
}

void test_remove (DataQueue * queue, int * testnum, int expected)
{
    printf("\nTest %d: REMOVE\n",(*testnum));

    int count = 0;
    DataQueue * temp = dataqueue_remove(queue);

    if (verbose)
    {
        printf("Parameters:\n");
        printf("\tqueue: 0x%X\n",queue);

        printf("\nRemoved data:\n");
        if (temp != NULL)
        {
            printf("\tRemoved datatype: 0x%X\n",temp->datatype);
            printf("\tRemoved packetsize: %d\n",temp->size);
            printf("\tRemoved packet:\n");
            for (count = 0; count < temp->size; count++)
            {
                printf("\t\t%d\n",temp->data[count]);
            }
        }
        else
        {
            printf("\tEmpty Queue\n");
        }
        printf("\tExpected Size: %d\n",expected);
    }

    printf("Test %d Result: %s\n",(*testnum)++,(g_dataqueue_size==expected ? "PASS" : "FAIL"));
}

void test_peek (DataQueue * queue, int * testnum, int expected)
{
    printf("\nTest %d: PEEK\n",(*testnum));

    int count = 0;
    DataQueue * temp = dataqueue_peek(queue);

    if (verbose)
    {
        printf("Parameters:\n");
        printf("\tqueue: 0x%X\n",queue);

        printf("\nPeeked data:\n");
        if (temp != NULL)
        {
            printf("\tPeeked datatype: 0x%X\n",temp->datatype);
            printf("\tPeeked packetsize: %d\n",temp->size);
            printf("\tPeeked packet:\n");
            for (count = 0; count < temp->size; count++)
            {
                printf("\t\t%d\n",temp->data[count]);
            }
        }
        else
        {
            printf("\tEmpty Queue\n");
        }
        printf("\tExpected Size: %d\n",expected);
    }

    printf("Test %d Result: %s\n",(*testnum)++,(g_dataqueue_size==expected ? "PASS" : "FAIL"));
}

void test_packetgen (DataQueue * queue, int * testnum, int expected)
{
    printf("\nTest %d: PACKETGEN\n",(*testnum));
    uint8_t packet[DATA_PACKET_SIZE];
    protocol_packet_generator(packet,queue);
    if (verbose)
    {
        int count = 0;
        printf("\nSOURCE ID: 0x%X%X\n",packet[0],packet[1]);
        printf("DEST ID: 0x%X%X\n",packet[2],packet[3]);
        printf("SIZE: %d\n",packet[4]);
        printf("PACKET:\n");
        for (count = 5; count < packet[4]-1; count++)
        {
            printf("\t%d\n",packet[count]);
        }
        printf("END: 0x%X\n",packet[packet[4]-1]);
    }
    printf("Test %d Result: %s\n",(*testnum)++,(packet[4]==expected ? "PASS" : "FAIL"));
}

void display_queue (DataQueue * queue)
{
    int count = 0;
    int i = 0;

    printf("\nQUEUE:\n");
    for (count = 0; count < g_dataqueue_size; count++)
    {
        int index = (g_dataqueue_front+count)%DATA_QUEUE_MAX;
        printf("\nStruct %d:\n",count);
        printf("\tdatatype: 0x%X\n",queue[index].datatype);
        printf("\tsize: %d\n",queue[index].size);
        printf("\tpacket:\n");
        for (i = 0; i < queue[index].size; i++)
        {
            printf("\t\t%d\n",queue[index].data[i]);
        }
    }
}
