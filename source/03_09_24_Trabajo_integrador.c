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
#define USR_DEBUG 0
#define TEST_STEPPER 1
#define AS5600_ON 0


int main(void) {
    uint16_t dev = VL53L1X_ADDRESS;
    uint32_t baud = 400000;
    uint32_t frecuency = 12000000;
    uint8_t status;
    menu_t menu = m_active;

    // Variables del vl53l1x
    laser_data_t lidar_data;
    uint16_t buffer_dist[10] = {0};
    laser_dist_mode lidar_mode = short_distance;

    // Variables del as6500
    uint16_t magnetic_angle = 0;

    // Vars stepper
    direction_t stepper_dir = CW;
    uint16_t mot_angle = 0;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_BootClockFRO18M();
    if (USR_DEBUG)
      BOARD_InitDebugConsole();

    init_gpio();
    init_systick(1000);
    
    init_bipolar_stepper(stepper_dir);
    select_micro_steps(full_step);

    if (TEST_STEPPER)
      test_bipolar_stepper();
    // Rx: 17   | Tx: 16
    init_SWM_USART(usart_port, kSWM_PortPin_P0_17, kSWM_PortPin_P0_16);
    // SCL: 18  | SDA: 19
    init_i2c1(kSWM_PortPin_P0_18, kSWM_PortPin_P0_19, baud, frecuency);
    init_vl53l1x(dev, lidar_mode);


    if (AS5600_ON && refresh_magnet_status()) {
      mot_angle = get_angle_position();
      if (USR_DEBUG)
        PRINTF("El valor del angulo es %i:", mot_angle);
      sprintf(msg_usart, "El valor del angulo es %i\n", mot_angle);
      USART_WriteBlocking(usart_port, msg_usart, strlen(msg_usart) - 1);
    }

    while(1) {
      select_menu();
      if (menu == m_active) {

        lidar_data = get_data_laser(dev);
        #if AS5600_ON
          if (refresh_magnet_status()) {
            magnetic_angle = get_angle_position();
            
            sprintf(msg_usart, "El valor del angulo es %i\n", magnetic_angle);
            USART_WriteBlocking(usart_port, msg_usart, strlen(msg_usart) - 1);
          } else {
            PRINTF("Error en la lectura del iman:\nMD: %d\tML: %d\t MH: %d\n", md, ml, mh);
          }
        #endif

        send_laser_uart(lidar_data, usart_port);

        if (move_bipolar_angle(MOT_ANGLE_PER_READ)) {
          if (USR_DEBUG)
            printf("El motor no esta realizando pasos");
        }
        
        
        mot_angle += MOT_ANGLE_PER_READ;
      }
    }
    return 0;
}

void select_menu() {
  if(!strcmp("ACTIVE", buffer_usart))
    menu = m_active;
  else if (!strcmp("IDLE", buffer_usart))
    menu = m_idle
  
}