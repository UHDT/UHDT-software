#ifndef GLOBALS_H
#define GLOBALS_H

#include "protocol.h"

#define TRUE 1
#define FALSE 0

#ifdef MAIN
// variables for main
DroneData g_data;
#else
// extern variables
extern DroneData g_data;
#endif

#endif
