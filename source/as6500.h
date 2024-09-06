#include "def.h"
// #define AS5600_ADDRESS 0x3C
#define AS5600_ADDRESS 0x36

extern uint8_t magnet_status, md, ml, mh;

extern uint8_t x[3];

void get_magnet_status(void);