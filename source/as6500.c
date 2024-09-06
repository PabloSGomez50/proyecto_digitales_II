#include "as6500.h"

void get_magnet_status() {
    request_i2c_data(AS5600_ADDRESS, 0x0B, x, 1);
    magnet_status = (x[0]>> 3) & 0x7;
    mh  = magnet_status & 0b001;
    ml = (magnet_status & 0b010) >> 1;
    md = (magnet_status & 0b100) >> 2;
}

uint16_t get_angle_position() {
    request_i2c_data(AS5600_ADDRESS, 0x0C, x, 2);
    uint16_t raw_value = (x[0] & 0x0F) * 256 + x[1];
    uint16_t angle = raw_value * (360 / 4095);
    return angle;
}