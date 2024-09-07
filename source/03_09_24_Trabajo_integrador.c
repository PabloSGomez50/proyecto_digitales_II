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
#include "stepper.h"

int main(void) {
    uint16_t dev = VL53L1X_ADDRESS;
    uint32_t baud = 400000;
    uint32_t frecuency = 12000000;
    uint8_t status;

    // Variables del vl53l1x
    struct laser_data_t lidar_data;
    uint16_t buffer_dist[255] = {0};
    laser_dist_mode lidar_mode = short_distance;

    // Variables del as6500
    uint16_t magnetic_angle = 0;


    // Vars stepper
    uint16_t mot_angle = 0;
    uint8_t current_step = 0;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_BootClockFRO18M();
    BOARD_InitDebugConsole();
    init_systick(1000);
    init_vl53l1x(dev, lidar_mode);
    init_i2c1(kSWM_PortPin_P0_26, kSWM_PortPin_P0_27, baud, frecuency);

    init_stepper();

    mot_angle = get_angle_position();

    while(1) {

      lidar_data = get_data_laser(dev);
      refresh_magnet_status();
      if (md & !ml & !mh) {
        magnetic_angle = get_angle_position();
      } else {
        printf("Error en la lectura del iman:\nMD: %d\tML: %d\t MH: %d", md, ml, mh);
      }

      if (lidar_data.ready != 0){
        printf("%u, %u, %u, %u, %u\r\n", lidar_data.range, lidar_data.distance, lidar_data.signal_rate, lidar_data.ambient_light, lidar_data.spad_num);
      } else {
        printf("Data not ready %i", lidar_data.ready);
        delay_mseg(20);
      }



      current_step++;
      make_step(current_step);
      mot_angle += MOT_ANGLE_PER_STEP;
    }
    return 0 ;
}
