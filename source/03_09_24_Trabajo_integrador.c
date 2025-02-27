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
#include "motors.h"

#define USR_DEBUG 0
#define TEST_STEPPER 0
#define TEST_USART 0
#define AS5600_ON 1
#define STEPPER_MOTOR_DEFAULT 0

void select_menu(menu_t * menu);
void check_usart_ajustments();
uint16_t get_mot_angle();
uint8_t steps_per_read = 3;
uint16_t max_steps = 0;
uint16_t i_steps = 0;
uint16_t mot_angle = 0;


// Variables del as6500
uint16_t magnetic_angle = 0;
uint16_t last_magnetic_angle = 0;
uint8_t diff_angle = 0;
uint16_t magnetic_full_turns = 0;

// Vars stepper
direction_t motor_dir = CW;
uint8_t motor_stepper_mode = STEPPER_MOTOR_DEFAULT;

int main(void) {
    uint16_t dev = VL53L1X_ADDRESS;
    uint32_t baud = 400000;
    uint32_t frecuency = 12000000;
    uint8_t status;
    menu_t menu = m_idle;

    // Variables del vl53l1x
    laser_data_t lidar_data;
    laser_dist_mode lidar_mode = short_distance;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_BootClockFRO18M();
    if (USR_DEBUG)
      BOARD_InitDebugConsole();

    init_gpio();
    init_systick(1000);
    
    init_bipolar_stepper(motor_dir);
    // if(motor_stepper_mode) {
    //   init_bipolar_stepper(motor_dir);
    //   select_micro_steps(full_step);
    // } else {
    //   init_dc_motor();
    // }
  

    if (TEST_STEPPER)
      test_bipolar_stepper();
    // Rx: 17   | Tx: 16
    init_SWM_USART(USART_PORT, kSWM_PortPin_P0_17, kSWM_PortPin_P0_16);
    while (TEST_USART) {
      select_menu(&menu);
      // sprintf(msg_usart, "Menu test\n");
      // USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
      delay_mseg(100);
    }
    // SCL: 19  | SDA: 20
    init_i2c1(kSWM_PortPin_P0_19, kSWM_PortPin_P0_20, baud, frecuency);
    init_vl53l1x(dev, lidar_mode);

    #if AS5600_ON
      init_as5600_dir();
      set_as5600_dir(motor_dir);
    #endif

    if (AS5600_ON && refresh_magnet_status()) {
      mot_angle = get_angle_position() * 10 / MOT_RATIO;
      last_magnetic_angle = mot_angle;
      if (USR_DEBUG)
        PRINTF("El valor del angulo es: %i\n", mot_angle);
      // sprintf(msg_usart, "El valor del angulo es %i\r\n", mot_angle);
      // USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    }

    while(1) {
      if (strlen(buffer_usart) > 0) {
        check_buffer_restart();
        check_usart_ajustments();
      }
      
      if (strlen(buffer_usart) > 0) {
        select_menu(&menu);
      }

      if (menu == m_active) {
        lidar_data = get_data_laser(dev);
        send_laser_uart(lidar_data, USART_PORT, mot_angle);

        mot_angle = get_mot_angle();
        
        if (motor_stepper_mode && move_bipolar_steps(steps_per_read)) {
          if (USR_DEBUG)
            printf("El motor no esta realizando pasos");
        }
      
      }
      else if (menu == m_steps)
      {
        if(i_steps < max_steps) {
          lidar_data = get_data_laser(dev);
          send_laser_uart(lidar_data, USART_PORT, mot_angle / MOT_RATIO);
          
          if(motor_stepper_mode) {
            move_bipolar_steps(steps_per_read);
          }
          mot_angle = get_mot_angle();
          // mot_angle += MOT_ANGLE_PER_STEP * steps_per_read;
          i_steps += steps_per_read;
          // make_bipolar_step();
          // mot_angle += MOT_ANGLE_PER_STEP;
          // i_steps ++;
        } else {
          menu = m_idle;
          if (motor_stepper_mode) {
            stop_stepper();
          } else { 
            stop_dc_motor();
          }
        }
      }
    }
    return 0;
  }
  
uint16_t get_mot_angle() {
  if (!AS5600_ON)
    return mot_angle + MOT_ANGLE_PER_STEP * steps_per_read;
    
  if (refresh_magnet_status()) {
    magnetic_angle = get_angle_position() * 10 / MOT_RATIO;
    // diff_angle = abs(magnetic_angle - last_magnetic_angle);
    
    if (abs(magnetic_angle - last_magnetic_angle) > 900) {
      // diff_angle = abs(1800 - diff_angle);
      magnetic_full_turns++;
    }
    // if (motor_dir == CCW)
    //   magnetic_angle = 1800 - magnetic_angle;
      
    last_magnetic_angle = magnetic_angle;
    return magnetic_full_turns * 1800 + magnetic_angle;
  }
  return mot_angle;
}

void check_usart_ajustments() {
  if (buffer_usart[0] == 'M' && buffer_usart[strlen(buffer_usart) - 1] == '.') {
    uint8_t match = sscanf(buffer_usart, "M%u.", &steps_per_read);
    if (match) {
      sprintf(msg_usart, "Resize steps per read: %i\r\n", steps_per_read);
      USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
      reset_usart();
    }
    return;
  }
  if (!strcmp("STATUS", buffer_usart)) {
    sprintf(msg_usart, "Status alive, steps: %d\r\n", steps_per_read);
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    reset_usart();
    return;
  }
  if (!strcmp("RESET", buffer_usart)) {
    mot_angle = 0;
    sprintf(msg_usart, "Reset motor angle\r\n");
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    reset_usart();
    return;
  }
  if (!strcmp("ANGLE", buffer_usart)) {
    sprintf(msg_usart, "Current angle: %i\r\n", mot_angle);
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    reset_usart();
    return;
  }
  if (!strcmp("STEPPER_MOTOR", buffer_usart)) {
    motor_stepper_mode = 1;
    sprintf(msg_usart, "Stepper mode: %i\r\n", motor_stepper_mode);
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    reset_usart();
    return;
  }
  if (!strcmp("DC_MOTOR", buffer_usart)) {
    motor_stepper_mode = 0;
    sprintf(msg_usart, "Stepper mode: %i\r\n", motor_stepper_mode);
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    reset_usart();
    return;
  }
}

void select_menu(menu_t * menu) {
  if(!strcmp("ACTIVE", buffer_usart)) {
    reset_usart();
    if(motor_stepper_mode)
      start_stepper();
    else
      start_dc_motor(motor_dir);
  
    sprintf(msg_usart, "New menu selected: ACTIVE\r\n");
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    *menu = m_active;
    return ;
  }
  if (!strcmp("IDLE",  buffer_usart)) {
    reset_usart();
    if(motor_stepper_mode)
      stop_stepper();
    else
      stop_dc_motor();
    
    sprintf(msg_usart, "New menu selected: IDLE\r\n");
    USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
    *menu = m_idle;
    return ;
  }
  if(motor_stepper_mode) {
    if(buffer_usart[0] == 'R' && buffer_usart[strlen(buffer_usart) - 1] == '.') {
      sscanf(buffer_usart, "R%u.", &max_steps);
      i_steps = 0;
      
      sprintf(msg_usart, "New menu selected: FIXED STEPS %i\n", max_steps);
      USART_WriteBlocking(USART_PORT, msg_usart, strlen(msg_usart) - 1);
      reset_usart();
      start_stepper();
      *menu = m_steps;
      return ;
    }
  }
}