#include "as6500.h"

uint8_t magnet_status, md, ml, mh, buffer_as6500[3];


void init_as5600_dir() {
    GPIO_PortInit(GPIO, AS5600_DIR_PORT);
    GPIO_PinInit(GPIO, AS5600_DIR_PORT, AS5600_DIR_PIN, &out_config);
}

void set_as5600_dir(uint8_t dir) {
    GPIO_PinWrite(GPIO, AS5600_DIR_PORT, AS5600_DIR_PIN, dir);
}

uint8_t refresh_magnet_status() {
    request_i2c_data(AS5600_ADDRESS, 0x0B, buffer_as6500, 1);
    magnet_status = (buffer_as6500[0]>> 3) & 0x7;
    mh  = magnet_status & 0b001;
    ml = (magnet_status & 0b010) >> 1;
    md = (magnet_status & 0b100) >> 2;

    return md & !ml & !mh;
}

uint16_t get_angle_position() {
    request_i2c_data(AS5600_ADDRESS, 0x0C, buffer_as6500, 2);
    uint16_t raw_value = (buffer_as6500[0] & 0x0F) * 256 + buffer_as6500[1];
    uint16_t angle = raw_value * (360 / 4095);
    return angle;
}