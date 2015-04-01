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

#include "globals.h"
#include "xbee.h"

// Old macros
#define REQUEST 0xAA
#define TRANSMIT 0xFF

#define LATITUDE 0xA1
#define LONGITUDE 0xA2
#define ALTITUDE 0xBA
#define VELOCITY 0xCA
#define ORIENTATION 0xDA
#define POWER 0xEA
#define SIGNAL_STRENGTH 0xFA

#define DESTINATION 0xAF

// Queue Macros
#define DATA_PACKET_SIZE 240
#define SOURCE_ID 0xDEAF
#define DEST_ID 0x3D2F
#define PACKET_END 0xED

volatile uint8_t g_packet[110];

void protocol_init_data();
void protocol_tx_data();
void protocol_rx_data();
void protocol_packet_generator ();
void protocol_data_packet_generator ();
void protocol_debug();
#endif
