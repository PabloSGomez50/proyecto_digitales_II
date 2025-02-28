#ifndef DEF_H_
#define DEF_H_

#include <stdio.h>
#include <stdint.h>

#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "LPC845.h"
#include "fsl_adc.h"
#include "fsl_debug_console.h"
#include "fsl_common.h"
#include "fsl_adc.h"
#include "fsl_power.h"
#include "fsl_swm.h"
#include "fsl_iocon.h"
#include "fsl_usart.h"
#include "fsl_i2c.h"


#define USART_PORT USART1
#define ADC_CHANNEL 1

#define TIEMPO_ANTIRREBOTE 40
#define USR_BTN 4
#define ISP_BTN 12
#define RED_PIN 2
#define GREEN_PIN 0
#define BLUE_PIN 1

#define W_LED_RED(x) 	GPIO_PinWrite(GPIO, 1, RED_PIN, x)
#define W_LED_BLUE(x) 	GPIO_PinWrite(GPIO, 1, BLUE_PIN, x)
#define W_LED_GREEN(x) 	GPIO_PinWrite(GPIO, 1, GREEN_PIN, x)

typedef enum {
	soltado = 0,
	pulsado_antirrebote = 1,
	pulsado = 2,
	soltado_antirrebote = 3,
} estado_boton_t;

typedef enum {
	m_active,
	m_idle,
	m_steps
} menu_t;

extern menu_t menu;

// Valores de I2C
extern uint32_t baud;
extern uint32_t frecuency;
extern volatile uint32_t flag_tick_main;
extern volatile uint32_t flag_tick_boton;


#define USART_SIZE 128
extern uint8_t buffer_usart[USART_SIZE];
extern uint8_t msg_usart[USART_SIZE];
extern uint8_t index_usart;

extern gpio_pin_config_t out_config;
extern gpio_pin_config_t out_config_low;
extern gpio_pin_config_t in_config;
// El valor maximo por arreglo de resistencias es 3.17V
#define TOTAL_ADC_BITS 3934 // (4096/3.3) * 0.13 = 161
#define MAX_BATTERY 840
extern adc_result_info_t adcResultInfoStruct;


void SysTick_Handler(void);
void USART0_IRQHandler(void);
void reset_usart(void);
void check_buffer_restart(void);

void init_systick(uint16_t div);
void delay_mseg(uint16_t mseg);

void init_adc(void);
void init_gpio(void);
void lectura_boton (uint8_t puerto, uint8_t boton, estado_boton_t *estado);
void init_i2c1(uint8_t pin_scl, uint8_t pin_sda, uint32_t baudRate, uint32_t frecuency);
void init_SWM_USART(USART_Type * port, uint8_t rx, uint8_t tx);

void request_i2c_data(uint16_t I2C_Addr, uint16_t register_addr, uint8_t * data, uint8_t size);

uint16_t get_battery_level();

#endif