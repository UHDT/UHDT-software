#ifndef XBEE_H
#define XBEE_H
#include <stm32f4xx.h>
#include <stm32f4xx_usart.h>
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "misc.h"

#define _START_BYTE 0x7e
#define _ESCAPE 0x7d
#define _XON 0x11
#define _XOFF 0x13

#define _TX_API_ID 0x10
#define _RX_API_ID 0x90
#define _TX_STATUS_ID 0x8B

#define _addr16 0xFFFE
#define _BROADCAST_RADIUS 0x00
#define _OPTIONS 0x00

#define _destH	0x0013A200  //base station's xbee
#define _destL	0x408B1D11
//	0x0013A200      drone's xbee
//	0x408B1D28

volatile uint8_t Rx_Buffer[110];     //single packet buffer for USART1



void init_USART1(uint32_t baudrate);

void send_byte(uint8_t b, int esc);

void xbee_send(uint8_t *payload, uint8_t payloadLength);

uint8_t get_frame_id();

void tx_request(uint8_t *datum, uint8_t datumLength);
#endif
