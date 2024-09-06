/*
 * Copyright 2016-2024 NXP
 * All rights reserved.
 */
 
/**
 * @file    03_09_24_Trabajo_integrador.c
 * @brief   Application entry point.
 */

#include "def.h"
#include "vl53l1x.h"
#include "as6500.h"

int main(void) {
    uint16_t dev = VL53L1X_ADDRESS;
    uint32_t baud = 400000;
    uint32_t frecuency = 12000000;
    uint8_t status;

    struct laser_data_t lidar_data;
    laser_dist_mode lidar_mode = short_distance;


  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_BootClockFRO18M();
    BOARD_InitDebugConsole();
    init_vl53l1x(dev, lidar_mode);


    while(1) {

      lidar_data = get_data_laser(dev);
      
      if (lidar_data.ready != 0){
        printf("%u, %u, %u, %u, %u\r\n", lidar_data.range, lidar_data.distance, lidar_data.signal_rate, lidar_data.ambient_light, lidar_data.spad_num);
      } else {
        printf("Data not ready %i", lidar_data.ready);
        // delay_mseg(20);
      }
    }
    return 0 ;
}
