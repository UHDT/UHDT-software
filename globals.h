#ifndef GLOBALS_H
#define GLOBALS_H

#include "protocol.h"

#ifdef MAIN
// variables for main
DroneData g_data;
#else
// extern variables
extern DroneData g_data;
#endif

#endif
