#ifndef WAYPOINTS_H
#define WAYPOINTS_H

#include <stdint.h>
#include <stdlib.h>

#include "macros.h"
#include "parsers.h"
#include "xbee.h"

void init_waypoints();
void parse_waypoints(uint8_t,uint8_t);

#ifdef MAIN
int waypoints[3];
#else
extern int waypoints[3];
#endif

#endif
