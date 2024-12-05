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
#define USR_DEBUG 1
#define TEST_STEPPER 0
#define AS5600_ON 0

menu_t select_menu();
void check_usart_ajustments();
uint8_t angle_per_read = MOT_ANGLE_PER_READ;
uint16_t max_steps = 0;
uint16_t i_steps = 0;

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
        check_usart_ajustments();
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

        send_laser_uart(lidar_data, usart_port, mot_angle);

        if (move_bipolar_angle(angle_per_read)) {
          if (USR_DEBUG)
            printf("El motor no esta realizando pasos");
        }
        
        mot_angle += angle_per_read;
      }
      else if (menu == m_steps)
      {
        while(i_steps < max_steps && menu == m_steps) {
          lidar_data = get_data_laser(dev);
          send_laser_uart(lidar_data, usart_port, mot_angle);

          if (move_bipolar_angle(angle_per_read)) {
            if (USR_DEBUG)
              printf("El motor no esta realizando pasos");
          }
          
          mot_angle += angle_per_read;
          i_steps++;
        }
      }
    }
    return 0;
}

void check_usart_ajustments() {
  if (buffer_usart[0] == 'M') {
    sscanf(buffer_usart, "M%u.", &angle_per_read);
  } else if (buffer_usart[0] == 'L') {
    // Change laser mode

  }
}

menu_t select_menu() {
  if(!strcmp("ACTIVE", buffer_usart))
    return m_active;
  if (!strcmp("IDLE", buffer_usart))
    return m_idle;
  if(buffer_usart[0] == 'R') {
    sscanf(buffer_usart, "R%u.", &max_steps);
    i_steps = 0;
    return m_steps;
  }
  
    return m_idle;
  
}