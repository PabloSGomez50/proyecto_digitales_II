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

#define USR_DEBUG 0
#define TEST_STEPPER 0
#define TEST_USART 0
#define AS5600_ON 1

menu_t select_menu();
void check_usart_ajustments();
uint8_t steps_per_read = 3;
uint16_t max_steps = 0;
uint16_t i_steps = 0;
uint16_t mot_angle = 0;
uint16_t max_angle = 0;

int main(void) {
    uint16_t dev = VL53L1X_ADDRESS;
    uint32_t baud = 400000;
    uint32_t frecuency = 12000000;
    uint8_t status;
    menu_t menu = m_idle;

    // Variables del vl53l1x
    laser_data_t lidar_data;
    laser_dist_mode lidar_mode = short_distance;

    // Variables del as6500
    uint16_t magnetic_angle = 0;
    uint16_t last_magnetic_angle = 0;
    uint8_t diff_angle = 0;
    uint16_t magnetic_full_turns = 0;

    // Vars stepper
    direction_t stepper_dir = CW;

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
    init_SWM_USART(USART_PORT, kSWM_PortPin_P0_17, kSWM_PortPin_P0_16);
    while (TEST_USART) {
      menu = select_menu();
      // sprintf(msg_usart, "Menu test\n");
      // USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
      delay_mseg(100);
    }
    // SCL: 18  | SDA: 19
    init_i2c1(kSWM_PortPin_P0_19, kSWM_PortPin_P0_20, baud, frecuency);
    // set_bipolar_direction(CCW);
    init_vl53l1x(dev, lidar_mode);
    // set_bipolar_direction(stepper_dir);

    #if AS5600_ON
      init_as5600_dir();
      set_as5600_dir(0);
    #endif

    if (AS5600_ON && refresh_magnet_status()) {
      mot_angle = get_angle_position() * 10 / MOT_RATIO;
      last_magnetic_angle = mot_angle;
      if (USR_DEBUG)
        PRINTF("El valor del angulo es: %i\n", mot_angle);
      sprintf(msg_usart, "El valor del angulo es %i\r\n", mot_angle);
      USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    }

    while(1) {
      if (strlen(buffer_usart) > 0)
        check_usart_ajustments();
      
      if (strlen(buffer_usart) > 0)
        menu = select_menu();

      if (menu == m_active) {
        lidar_data = get_data_laser(dev);
        send_laser_uart(lidar_data, USART_PORT, mot_angle);

        if (AS5600_ON && refresh_magnet_status()) {
          magnetic_angle = get_angle_position() * 10 / MOT_RATIO;
          // diff_angle = abs(magnetic_angle - last_magnetic_angle);
          last_magnetic_angle = magnetic_angle;

          if (abs(magnetic_angle - last_magnetic_angle) > 900) {
            // diff_angle = abs(1800 - diff_angle);
            magnetic_full_turns++;
            sprintf(msg_usart, "Se dio una vuelta completa del as5600: %d\r\n", magnetic_full_turns);
            USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
          }
          if (stepper_dir == CCW)
            magnetic_angle = 1800 - magnetic_angle;
          mot_angle = magnetic_full_turns * 1800 + magnetic_angle;
          // mot_angle += diff_angle;
          
          // sprintf(msg_usart, "El valor del angulo es %i\r\n", magnetic_angle);
          // USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
        } else {
          if (USR_DEBUG)
            PRINTF("Error en la lectura del iman:\nMD: %d\tML: %d\t MH: %d\n", md, ml, mh);
          mot_angle += MOT_ANGLE_PER_STEP * steps_per_read;
        }
        if (move_bipolar_steps(steps_per_read)) {
          if (USR_DEBUG)
            printf("El motor no esta realizando pasos");
        }
        
      }
      else if (menu == m_steps)
      {
        if(i_steps < max_steps) {
          lidar_data = get_data_laser(dev);
          send_laser_uart(lidar_data, USART_PORT, mot_angle / MOT_RATIO);
          
          move_bipolar_steps(steps_per_read);
          mot_angle += MOT_ANGLE_PER_STEP * steps_per_read;
          i_steps += steps_per_read;
          // make_bipolar_step();
          // mot_angle += MOT_ANGLE_PER_STEP;
          // i_steps ++;
        } else {
          menu = m_idle;
          stop_stepper();
        }
      }
    }
    return 0;
}

void check_usart_ajustments() {
  if (buffer_usart[0] == 'M' && buffer_usart[strlen(buffer_usart) - 1] == '.') {
    uint8_t match = sscanf(buffer_usart, "M%u.", &steps_per_read);
    if (match) {
      sprintf(msg_usart, "Resize steps per read: %i\r\n", steps_per_read);
      USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
      reset_usart();
    }
  } else if (!strcmp("STATUS", buffer_usart)) {
    sprintf(msg_usart, "Status alive, steps: %d\r\n", steps_per_read);
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    reset_usart();
  } else if (!strcmp("RESET", buffer_usart)) {
    mot_angle = 0;
    sprintf(msg_usart, "Reset motor angle\r\n");
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    reset_usart();
  } else if (!strcmp("ANGLE", buffer_usart)) {
    sprintf(msg_usart, "Current angle: %i\r\n", mot_angle);
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    reset_usart();
  }
}

menu_t select_menu() {
  if(!strcmp("ACTIVE", buffer_usart)) {
    // W_LED_RED(0);
    // W_LED_GREEN(1);
    // W_LED_BLUE(1);
    reset_usart();
    start_stepper();
    sprintf(msg_usart, "New menu selected: ACTIVE\n");
    max_angle += 3600;
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    return m_active;
  }
  if (!strcmp("IDLE",  buffer_usart)) {
    // W_LED_RED(1);
    // W_LED_GREEN(0);
    // W_LED_BLUE(1);
    reset_usart();
    stop_stepper();
    sprintf(msg_usart, "New menu selected: IDLE\n");
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    return m_idle;
  }
  if(buffer_usart[0] == 'R' && buffer_usart[strlen(buffer_usart) - 1] == '.') {
    // W_LED_RED(1);
    // W_LED_GREEN(1);
    // W_LED_BLUE(0);
    sscanf(buffer_usart, "R%u.", &max_steps);
    i_steps = 0;
    
    sprintf(msg_usart, "New menu selected: FIXED STEPS %i\n", max_steps);
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    reset_usart();
    start_stepper();
    return m_steps;
  }
    return m_idle;
  
}