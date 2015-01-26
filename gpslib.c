#include <stdio.h>
#include "gpslib.h"

extern int debug_count;

#ifdef COOIDE
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>
#include <stm32f4xx_tim.h>
#include <misc.h>


/******************************************************************
 * readNMEA() reads an entire NMEA string from the GPS on USART1.
 *****************************************************************/
void readNMEA(GPS* gps)
{
    int count;
    char c = 'A';

    strClear(gps->nmea);

    // note: maybe change to double quotes for chars
    for(count = 0; (c != '\n') && (c != '\r'); count++)
    {
	if(count == 399)
	{
	    do
	    {
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		c = USART_ReceiveData(USART1);
	    }
	    while ((c != '\n') && (c != '\r'));
	    gps->nmea[count] = c;
	    return;
	}

	else
	{
	    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	    c = USART_ReceiveData(USART1);

	    while(c != '$' && count == 0)
	    {
		 while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
		 c = USART_ReceiveData(USART1);
	    }

	    gps->nmea[count] = c;
	}
    }
}

/******************************************************************
 * readNMEAcharUSART1() reads just one char at a time from USART1.
 * It then returns that char.
 *****************************************************************/
char readNMEAcharUSART1(GPS* gps)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    return USART_ReceiveData(USART1);
}

/******************************************************************
 * getNMEA() gets the NMEA string from the GPS
 *****************************************************************/
void getNMEA(GPS* gps)
{
    char c;

    // Reads the NMEA from the GPS on USART1
    c = readNMEAcharUSART1(gps);
    if(c == '$')
    	gps->charCount = 0;

    gps->nmea[gps->charCount] = c;
    gps->charCount++;

    //while(gps->charCount < 300)
    while(c != '\n' && c != '\r')
    {
    	c = readNMEAcharUSART1(gps);
        if(c == '$')
        	gps->charCount = 0;
    	gps->nmea[gps->charCount] = c;
    	gps->charCount++;
    }

    gps->charCount = 0;
}

#ifdef INTERRUPT_DEBUG
void TIM2_IRQHandler()
{
	char c;

    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

		debug_count++;
		printf("%d",debug_count);
    }
}
#endif

#endif

int parseNMEA(char* c)
{
    if(strStarts(c,"$GPGGA"))
	return parseGGA(c);
    else
    {
#ifdef DEBUG
printf("DEBUG\n");
#endif
	return FALSE;
    }
}

int parseGGA(char* c)
{
    int count = 0;
    int placecount = 0;
    int numTensPlace = 10;

    int hour = 0;
    int min = 0;
    int sec = 0;
    double latitude = 0, latDecimal = 0;
    double longitude = 0, lonDecimal = 0;
    int fix = 0;
    int numSats = 0;
    float altitude = 0, altDecimal = 0;

    for(count = 0; c[count] != ','; count++);

// TIME (UTC)
    for(++count; c[count] != ','; count++)
    {
	if(c[count] == '\0' || ((c[count] < '0' || c[count] > '9') && c[count] != '.'))
	{
#ifdef DEBUG
printf("DEBUG1\n");
#endif
	    return FALSE;
	}
	if(placecount < HOUR)
	{
	    hour = hour*10 + strToInt(c[count]);
	}
	else if(placecount < MIN)
	{
	    min = min*numTensPlace + strToInt(c[count]);
	}
	else if(placecount < SEC)
	{
	    sec = sec*numTensPlace + strToInt(c[count]);
	}

	placecount++;
    }
    placecount = 0;

// START OF NEW LATITUDE
    for(++count; c[count] != '.' && c[count] != ','; count++)
    {
	if(c[count] == '\0' || (c[count] < '0' || c[count] > '9'))
	{
#ifdef DEBUG
printf("DEBUG2\n");
#endif
	    return FALSE;
	}
	latitude = latitude*numTensPlace + strToInt(c[count]);
    }
    if(c[count] == '.')
    {
	for(++count; c[count] != ','; count++)
	{
	    if(c[count] == '\0' || (c[count] < '0' || c[count] > '9'))
	    {
#ifdef DEBUG
printf("DEBUG3\n");
#endif
		return FALSE;
	    }
	    latDecimal = latDecimal*numTensPlace + strToInt(c[count]);
	    placecount++;
	}
	for(placecount; placecount > 0; placecount--)
	    latDecimal /= numTensPlace;
	latitude += latDecimal;
    }

    for(++count; c[count] != ','; count++)
    {
	if(c[count] == 'S')
	    latitude *= -1;
	else if(c[count] == 'N');
	else
	{
#ifdef DEBUG
printf("DEBUG4\n");
#endif
	    return FALSE;
	}
    }
// END OF NEW LATITUDE

// START OF NEW LONGITUDE
    for(++count; c[count] != '.' && c[count] != ','; count++)
    {
	if(c[count] == '\0' || (c[count] < '0' || c[count] > '9'))
	{
#ifdef DEBUG
printf("DEBUG5\n");
#endif
	    return FALSE;
	}
	longitude = longitude*numTensPlace + strToInt(c[count]);
    }
    if(c[count] == '.')
    {
	for(++count; c[count] != ','; count++)
	{
	    if(c[count] == '\0' || (c[count] < '0' || c[count] > '9'))
	    {
#ifdef DEBUG
printf("DEBUG6\n");
#endif
		return FALSE;
	    }
	    lonDecimal = lonDecimal*numTensPlace + strToInt(c[count]);
	    placecount++;
	}
	for(placecount; placecount > 0; placecount--)
	    lonDecimal /= numTensPlace;
	longitude += lonDecimal;
    }

    for(++count; c[count] != ','; count++)
    {
	if(c[count] == 'W')
	    longitude *= -1;
	else if(c[count] == 'E');
	else
	{
#ifdef DEBUG
printf("DEBUG7\n");
#endif
	    return FALSE;
	}
    }
// END OF NEW LONGITUDE

// FIX TO A SATELLITE
    for(++count; c[count] != ','; count++)
    {
	if(strToInt(c[count]) == 0)
	    fix = FALSE;
	else if(strToInt(c[count]) == 1)
	    fix = TRUE;
	else if(strToInt(c[count]) == 2)
		fix = 2;
	else
	{
#ifdef DEBUG
printf("DEBUG8\n");
#endif
	    return FALSE;
	}
    }

// NUMBER OF SATELLITES USED
    for(++count; c[count] != ','; count++)
    {
	if(c[count] == '\0' || (c[count] < '0' || c[count] > '9'))
	{
#ifdef DEBUG
printf("DEBUG9\n");
#endif
	    return FALSE;
	}
	numSats = numSats*numTensPlace + strToInt(c[count]);
    }

// HDOP
    for(++count; c[count] != ','; count++);

// ALTITUDE START
    for(++count; c[count] != '.' && c[count] != ','; count++)
    {
	if(c[count] == '\0' || (c[count] < '0' || c[count] > '9'))
	{
#ifdef DEBUG
printf("DEBUG10\n");
#endif
	    return FALSE;
	}
	altitude = altitude*numTensPlace + strToInt(c[count]);
    }
    if(c[count] == '.')
    {
	for(++count; c[count] != ','; count++)
	{
	    if(c[count] == '\0' || (c[count] < '0' || c[count] > '9'))
	    {
#ifdef DEBUG
printf("DEBUG11\n");
#endif
		return FALSE;
	    }
	    altDecimal = altDecimal*numTensPlace + strToInt(c[count]);
	    placecount++;
	}
	for(placecount; placecount > 0; placecount--)
	    altDecimal /= numTensPlace;
	altitude += altDecimal;
    }
// altitude end

    gps.hour = hour;
    gps.min = min;
    gps.sec = sec;
    gps.latitude = latitude;
    gps.longitude = longitude;
    gps.fix = fix;
    gps.numSats = numSats;
    gps.altitude = altitude;

    return TRUE;
}
