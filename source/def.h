#include <stdio.h>
#include <stdint.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "fsl_adc.h"
#include "fsl_power.h"
#include "fsl_swm.h"
#include "fsl_iocon.h"
#include "fsl_usart.h"
#include "fsl_i2c.h"

// Valores de I2C
extern uint32_t baud;
extern uint32_t frecuency;

void delay_mseg(int);

void init_i2c1(uint8_t pin_scl, uint8_t pin_sda, uint32_t baudRate, uint32_t frecuency);

void request_i2c_data(uint16_t I2C_Addr, uint16_t register_addr, uint8_t * data, uint8_t size);
