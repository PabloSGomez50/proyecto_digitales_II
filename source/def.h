#ifndef DEF_H_
#define DEF_H_

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


#define TIEMPO_ANTIRREBOTE 40

typedef enum {
	soltado = 0,
	pulsado_antirrebote = 1,
	pulsado = 2,
	soltado_antirrebote = 3,
} estado_boton_t;


// Valores de I2C
extern uint32_t baud;
extern uint32_t frecuency;
extern volatile uint32_t flag_tick_delay;
extern volatile uint32_t flag_tick_boton;

void init_systick(uint16_t div);
void delay_mseg(uint16_t msegs);

void init_i2c1(uint8_t pin_scl, uint8_t pin_sda, uint32_t baudRate, uint32_t frecuency);

void request_i2c_data(uint16_t I2C_Addr, uint16_t register_addr, uint8_t * data, uint8_t size);


void init_SWM_USART(uint8_t port, uint8_t rx, uint8_t tx);

#endif