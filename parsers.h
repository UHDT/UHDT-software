#ifndef PARSERS_H
#define PARSERS_H

#include <stdint.h>
#include "macros.h"

typedef void (*ParseFunction)(uint8_t,uint8_t);

typedef struct
{
    int size;
    ParseFunction PARSE_FUNC;
} Parser;

#ifdef MAIN
Parser * g_parser[NUM_DATA_TYPES] = {0};
#else
extern Parser * g_parser[NUM_DATA_TYPES];
#endif

int parse_data(uint8_t,uint8_t);

#endif
