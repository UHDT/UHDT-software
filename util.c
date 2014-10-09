#include "util.h"

void util_delay(uint32_t ticks)
{
    uint32_t i = 0;
    for (i = 0; i < ticks; i++)
    {
        SystemCoreClockUpdate();
    }
}
