#ifndef MACROS_H
#define MACROS_H

#define TRUE 1
#define FALSE 0

// Old protocol macros
#define REQUEST 0xAA
#define TRANSMIT 0xFF

// Data types
#define NUM_DATA_TYPES 0xFF
#define WAYPOINT 0x00
#define GPS 0x01
#define ALTITUDE 0xBA
#define VELOCITY 0xCA
#define ORIENTATION 0xDA
#define SIGNAL_STRENGTH 0xFA

#define DESTINATION 0xAF

// New protocol macros
#define DATA_PACKET_SIZE 10
#define PACKET_SIZE 240
#define SOURCE_ID 0xDEAF
#define DEST_ID 0x3D2F
#define PACKET_END 0xED

// Queue macros
#define DATA_QUEUE_MAX 100

#endif