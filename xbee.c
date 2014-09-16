#include "xbee.h"


void init_USART1(uint32_t baudrate)
{
        GPIO_InitTypeDef GPIO_InitStruct; // this is for the GPIO pins used as TX and RX
        USART_InitTypeDef USART_InitStruct; // this is for the USART1 initilization
        NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)

        /* enable APB2 peripheral clock for USART1
         * note that only USART1 and USART6 are connected to APB2
         * the other USARTs are connected to APB1
         */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

        /* enable the peripheral clock for the pins used by
         * USART1, PB6 for TX and PB7 for RX
         */
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

        /* This sequence sets up the TX and RX pins
         * so they work correctly with the USART1 peripheral
         */
        GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; // Pins 6 (TX) and 7 (RX) are used
        GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;                         // the pins are configured as alternate function so the USART peripheral has access to them
        GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;                // this defines the IO speed and has nothing to do with the baudrate!
        GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;                        // this defines the output type as push pull mode (as opposed to open drain)
        GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;                        // this activates the pullup resistors on the IO pins
        GPIO_Init(GPIOB, &GPIO_InitStruct);                                        // now all the values are passed to the GPIO_Init() function which sets the GPIO registers

        /* The RX and TX pins are now connected to their AF
         * so that the USART1 can take over control of the
         * pins
         */
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); //
        GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

        /* Now the USART_InitStruct is used to define the
         * properties of USART1
         */
        USART_InitStruct.USART_BaudRate = baudrate;                                // the baudrate is set to the value we passed into this init function
        USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
        USART_InitStruct.USART_StopBits = USART_StopBits_1;                // we want 1 stop bit (standard)
        USART_InitStruct.USART_Parity = USART_Parity_No;                // we don't want a parity bit (standard)
        USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
        USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
        USART_Init(USART1, &USART_InitStruct);                                        // again all the properties are passed to the USART_Init function which takes care of all the bit setting


        /* Here the USART1 receive interrupt is enabled
         * and the interrupt controller is configured
         * to jump to the USART1_IRQHandler() function
         * if the USART1 receive interrupt occurs
         */
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt

        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;                 // we want to configure the USART1 interrupts
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART1 interrupts
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                 // this sets the subpriority inside the group
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                         // the USART1 interrupts are globally enabled
        NVIC_Init(&NVIC_InitStructure);                                                         // the properties are passed to the NVIC_Init function which takes care of the low level stuff

        // finally this enables the complete USART1 peripheral
        USART_Cmd(USART1, ENABLE);
}


/*
    Send a byte through USART following the xbee API protocol.

    @param  b       the byte to be send
    @param  esc     flags whether or not to xor
*/
void sendByte(uint8_t b, int esc)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);

    if (esc && (b == _START_BYTE || b == _ESCAPE || b == _XON || b == _XOFF))
    {
        USART_SendData(USART1, _ESCAPE);
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, b ^ 0x20);
    }

    else
        USART_SendData(USART1, b);
}



/*
    Sends an xbee API frame through USART.

    @param  payload       array of API frame
    @param  payloadLength size of frame
*/
void xbee_send(uint8_t *payload, uint8_t payloadLength)
{
    uint8_t j;
    uint8_t checksum = 0;
	uint8_t msbLen = (payloadLength >> 8) & 0xff;
	uint8_t lsbLen = payloadLength  & 0xff;

    sendByte(_START_BYTE, 0);

	sendByte(msbLen, 1);
	sendByte(lsbLen, 1);

	sendByte(payload[0], 1);
	sendByte(payload[1], 1);

	checksum+= payload[0];
	checksum+= payload[1];


	for (j = 2; j < payloadLength; j++)
    {
		sendByte(payload[j], 1);
		checksum+= payload[j];
	}

	checksum = 0xff - checksum;

	sendByte(checksum, 1);

}



/*
    Generates a frame ID for an xbee API packet.
*/
uint8_t getFrameId()
{
	static uint8_t frameId = 0;
	
	frameId++;
	
	if ( frameId == 0 )
		frameId = 1;
		
	return frameId;
}




/*
    Generates and sends an xbee API Tx request for the given data.

    @param  datum           array of data to be sent
    @param  datumLength     size of data
*/
void Tx_Request(uint8_t *datum, uint8_t datumLength)
{
	uint8_t i;
    uint8_t request[datumLength + 14];

	request[0] = _TX_API_ID;
	request[1] = getFrameId();
	request[2] = (_destH >> 24) & 0xff;
	request[3] = (_destH >> 16) & 0xff;
	request[4] = (_destH >> 8) & 0xff;
	request[5] = _destH & 0xff;
	request[6] = (_destL >> 24) & 0xff;
	request[7] = (_destL >> 16) & 0xff;
	request[8] = (_destL >> 8) & 0xff;
	request[9] = _destL & 0xff;
	request[10] = (_addr16 >> 8) & 0xff;
	request[11] = _addr16 & 0xff;
	request[12] = _BROADCAST_RADIUS;
	request[13] = _OPTIONS;

	for ( i = 14; i < datumLength+14; i++ )
		request[i] = datum[i - 14];

	xbee_send(request, sizeof(request));

}


/*
	Echoes an Rx packet.  Purely for debugging purposes.
*/
void echo()
{
    uint16_t pkSize = Rx_Buffer[1] * 256 + Rx_Buffer[2] + 4;

    if ( Rx_Buffer[3] == _RX_API_ID )
        Tx_Request(Rx_Buffer, pkSize);
}



/*
	**Known flaw:  Receiving multiple escape characters
	will will trigger xor if-statement multiple times resulting
	in incorrectly read data.

	Interrupt that handles xbee API packets received through
	USART1.
	Reconstructs the packet and places it in the Rx buffer.
	**The buffer only stores one packet at a time.**
	
	Any type of data extraction on Rx packets *could* be handled
	in the else section of this function.
*/
void USART1_IRQHandler(void)
{
	// check if the USART1 receive interrupt flag was set
	if( USART_GetITStatus(USART1, USART_IT_RXNE) )
    {
		static uint16_t cnt = 0; // index of received packet
		static uint16_t expSize = 200;  // expected packet size filled with temp value until MSB and LSB are read
		char t = USART1->DR; // the character from the USART1 data register is saved in t

		// keep storing bytes until the checksum is stored
		if( cnt != expSize - 1)
        {
			Rx_Buffer[cnt] = t; //store data in Rx buffer

			//xor byte if necessary
			if ( Rx_Buffer[cnt-1] == _ESCAPE )
            {
                Rx_Buffer[cnt-1] = t ^ 0x20;
                cnt--;
            }

			if ( cnt == 3 )
                expSize = (uint16_t)Rx_Buffer[1] * 256 + Rx_Buffer[2] + 4;    // establish expected packet size

			cnt++;
		}

		//at this point, may need to flag that there is a new packet in the buffer
		else
        { // otherwise reset values
			echo();
			cnt = 0;
			expSize = 200;
		}
	}
}