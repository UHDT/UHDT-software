#include "waypoints.h"

void init_waypoints()
{
    g_parser[WAYPOINT] = (Parser*) malloc(sizeof(Parser));
    g_parser[WAYPOINT]->size = 3;
    g_parser[WAYPOINT]->PARSE_FUNC = &parse_waypoints;
}

void parse_waypoints(uint8_t index, uint8_t size)
{
    int count = 0;

    for (count = 0; count < size; count++)
    {
        waypoints[count] = Rx_Buffer[count+index];
    }
}
