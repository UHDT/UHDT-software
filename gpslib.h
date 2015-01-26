#define LINE1 0
#define LINE2 1
#define NOFLAG 2
#define TRUE 1
#define FALSE 0

#define HOUR 2
#define MIN 4
#define SEC 6

#define COOIDE
//#define DEBUG

typedef struct
{
	// Used for storing gps NMEA string until the full string
	// is read
    char nmea[400];

    // Lines used for storing data
    char line1[400];
    char line2[400];

    int charCount;

    // Indicates which line is currently being read to
    int currentLine;

    // Indicates whether or not at least one of the lines is ready
    // to be read. A value of 1 means line 1 is ready, 2 means line 2
    // is ready, and 3 means both lines are ready. 0 means no lines are
    // ready.
    int receiveFlag;

    int hour;
    int min;
    int sec;
    double latitude;
    double longitude;
    int fix;
    int numSats;
    float altitude;
} GPS;

// GLOBALS
GPS gps;
