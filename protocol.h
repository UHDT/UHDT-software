#ifndef PROTOCOL_H
#define PROTOCOL_H

#ifndef TESTBENCH
#include <stm32f4xx.h>
#include <stm32f4xx_usart.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"
#endif

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "macros.h"
#include "xbee.h"
#include "dataqueue.h"

// Old Protocol prototypes and global
volatile uint8_t g_packet[110];

void protocol_init_data();
void protocol_tx_data();
void protocol_rx_data();
void protocol_debug();

// New queue struct, prototypes
typedef struct
{
    uint8_t latitude;
    uint8_t longitude;
    uint8_t altitude;
    uint8_t velocity;
    uint8_t orientation;
    uint8_t power;
    uint8_t signal_strength;

    uint8_t dest_lat;
    uint8_t dest_lon;
} DroneData;

#ifdef MAIN
// variables for main
DroneData g_data;
#else
// extern variables
extern DroneData g_data;
#endif

uint8_t * protocol_packet_generator (uint8_t*,DataQueue*);
int protocol_data_packet_generator (uint8_t[],int*,DataQueue*);
#endif
