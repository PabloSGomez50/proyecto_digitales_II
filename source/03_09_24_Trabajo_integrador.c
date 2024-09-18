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

#define usart_port USART2


int main(void) {
    uint16_t dev = VL53L1X_ADDRESS;
    uint32_t baud = 400000;
    uint32_t frecuency = 12000000;
    uint8_t status;

    // Variables del vl53l1x
    struct laser_data_t lidar_data;
    uint16_t buffer_dist[10] = {0};
    laser_dist_mode lidar_mode = short_distance;

    // Variables del as6500
    uint16_t magnetic_angle = 0;
    uint16_t mot_angle = 0;


    // Vars steppera
    uint8_t current_step = 0;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_BootClockFRO18M();
    BOARD_InitDebugConsole();
    init_systick(1000);
    init_SWM_USART(usart_port, kSWM_PortPin_P0_17, kSWM_PortPin_P0_16);
    init_i2c1(kSWM_PortPin_P0_18, kSWM_PortPin_P0_19, baud, frecuency);
    init_vl53l1x(dev, lidar_mode);
    mot_angle = get_angle_position();
    // PRINTF("El valor del angulo es %i:", mot_angle);
    sprintf(msg_usart, "El valor del angulo es %i\n", mot_angle);
    USART_WriteBlocking(usart_port, msg_usart, strlen(msg_usart) - 1);

    // init_stepper();

    while(1) {

      lidar_data = get_data_laser(dev);
      refresh_magnet_status();
      if (md & !ml & !mh) {
        magnetic_angle = get_angle_position();
      } else {
        PRINTF("Error en la lectura del iman:\nMD: %d\tML: %d\t MH: %d\n", md, ml, mh);
      }

      if (lidar_data.ready != 0){
        // PRINTF("%u, %u, %u, %u, %u\r\n", lidar_data.range, lidar_data.distance, lidar_data.signal_rate, lidar_data.ambient_light, lidar_data.spad_num);
        sprintf(msg_usart, "%u, %u, %u, %u, %u\n", lidar_data.range, lidar_data.distance, lidar_data.signal_rate, lidar_data.ambient_light, lidar_data.spad_num);
        USART_WriteBlocking(usart_port, msg_usart, strlen(msg_usart));
      } else {
        // PRINTF("Data not ready %i", lidar_data.ready);
        sprintf(msg_usart, "Data not ready %i\n", lidar_data.ready);
        USART_WriteBlocking(usart_port, msg_usart, strlen(msg_usart));
        delay_mseg(20);
      }



      current_step++;
      // make_step(current_step);
      mot_angle += MOT_ANGLE_PER_STEP;
    }
    return 0 ;
}
