#include "def.h"
// #define AS5600_ADDRESS 0x3C
#define AS5600_ADDRESS 0x36

extern uint8_t magnet_status, md, ml, mh;

extern uint8_t buffer_as6500[3];

// Direction polarity (GND = values increase clockwise,
// VDD = values increase counterclockwise)

void refresh_magnet_status(void);

uint16_t get_angle_position();