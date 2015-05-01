#include "parsers.h"

int parse_data(uint8_t datatype, uint8_t index)
{
    if (g_parser[datatype] == 0)
        return 0;
    g_parser[datatype]->PARSE_FUNC(index,g_parser[datatype]->size);

    return g_parser[datatype]->size;
}
