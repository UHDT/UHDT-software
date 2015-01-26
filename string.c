#include "string.h"
#include "gpslib.h"

int strLength(char * str)
{
    int count = 0;
   
    while(str[count] != '\0')
        count ++;
    return count;
}

void strClear(char * str)
{
    int count = 0;

    while(str[count] != '\0')
    {
        str[count] = '\0';
        count++;
    }
}

void strCopy(char * strTo, char * strFrom)
{
    int count = 0, strToLength = 0;

    strClear(strTo);
    while(strFrom[count] != '\0')
    {
        strTo[count] = strFrom[count]; 
        count++;
    }
}

void strDisplay(char * str)
{
    int count = 0;

    while(str[count] != '\0')
    {
	#ifdef COOIDE
	PrintChar(str[count]);
	#else
	printf("%c",str[count]);
	#endif
    	count++;
    }
}

int strEqual(char* str1, char* str2)
{
    int count;

    for(count = 0; str1[count] != '\0' && str2[count] != '\0'; count++)
    {
    	if(str1[count] != str2[count])
    		return FALSE;
    }

    if(str1[count] == '\0' && str2[count] == '\0')
        return TRUE;
    else return FALSE;
}

int strContains(char* str, char* compare)
{
    int count, countComp,countStr, match = TRUE;
    
    for(count = 0; str[count] != '\0' && match == FALSE; count++)
    {
	    match = TRUE;
	    countStr = count;
	    for(countComp = 0; compare[countComp] != '\0' && str[countStr] != '\0'; countComp++)
	    {
	        if(str[countStr] != compare[countComp])
		        match = FALSE;
	        countStr++;
	    }
    }
    
    return match;
}

int strStarts(char* str, char* compare)
{
    int count = 0, strcount = 0;

    if(strLength(str) < strLength(compare))
    	return FALSE;

    for(count = 0; compare[count] != '\0'; count++)
    {
	if(str[strcount] != compare[count])
	    return FALSE;
	strcount++;
    }

    return TRUE;
}

int strToInt(char c)
{
    if(c >= '0' && c <= '9')
	return c-48;
    else
	return;
}
