#include "def.h"


volatile uint32_t flag_tick_delay = 0;
volatile uint32_t flag_tick_boton = 0;
volatile uint32_t flag_tick_usart = 0;
volatile uint32_t ref_tick;


uint8_t buffer_usart[USART_SIZE] = "";
uint8_t msg_usart[USART_SIZE] = "";
uint8_t index_usart = 0;
gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 1};
gpio_pin_config_t in_config = {kGPIO_DigitalInput};


void init_systick(uint16_t div) {
	(void) SysTick_Config(SystemCoreClock/div);
}


void SysTick_Handler(void)
{
	if (flag_tick_delay >= 4294900000)
		flag_tick_delay = 0;
	flag_tick_delay++;
	flag_tick_usart++;
	flag_tick_boton++;
}

void USART1_IRQHandler(void) {
	if (flag_tick_usart > 500) {
		index_usart = 0;
		flag_tick_usart = 0;
	}
	if (index_usart < USART_SIZE) {
		uint8_t c = USART_ReadByte(USART_PORT);
		if (c != '\r' && c != '\n') {
			buffer_usart[index_usart] = c;
			index_usart++;
		} else {
			buffer_usart[index_usart] = '\0';
		}
	}
}

void reset_usart(void) {
	for (int i = 0; i < index_usart; i++)
		buffer_usart[i] = '\0';
	index_usart = 0;
}

// Normal functions
void init_gpio(void) {
	
    GPIO_PortInit(GPIO, 0);
    GPIO_PortInit(GPIO, 1);
    GPIO_PinInit(GPIO, 1, RED_PIN, &out_config);
    GPIO_PinInit(GPIO, 1, GREEN_PIN, &out_config);
    GPIO_PinInit(GPIO, 1, BLUE_PIN, &out_config);
    GPIO_PinInit(GPIO, 0, USR_BTN, &in_config);
    GPIO_PinInit(GPIO, 0, ISP_BTN, &in_config);
}

void delay_mseg(uint16_t mseg){
	ref_tick = flag_tick_delay + mseg;
	while(flag_tick_delay <= ref_tick);
}

void init_i2c1(uint8_t pin_scl, uint8_t pin_sda, uint32_t baudRate, uint32_t frecuency) {
    CLOCK_Select(kI2C1_Clk_From_MainClk);
    CLOCK_EnableClock(kCLOCK_Swm);
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SDA, pin_sda);
    SWM_SetMovablePinSelect(SWM0, kSWM_I2C1_SCL, pin_scl);
    CLOCK_DisableClock(kCLOCK_Swm);
    i2c_master_config_t masterConfig;
    /*
      * masterConfig.debugEnable = false;
      * masterConfig.ignoreAck = false;
      * masterConfig.pinConfig = kI2C_2PinOpenDrain;
      * masterConfig.baudRate_Bps = 100000U;
      * masterConfig.busIdleTimeout_ns = 0;
      * masterConfig.pinLowTimeout_ns = 0;
      * masterConfig.sdaGlitchFilterWidth_ns = 0;
      * masterConfig.sclGlitchFilterWidth_ns = 0;
      */
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = baudRate;
    I2C_MasterInit(I2C1, &masterConfig, frecuency);
}

void request_i2c_data(uint16_t I2C_Addr, uint16_t register_addr, uint8_t * data, uint8_t size) {
    uint8_t buffer_size = 1;
    uint16_t txBuff[1];
    int32_t r;

    if (kStatus_Success == I2C_MasterStart(I2C1, I2C_Addr, kI2C_Write))
    {
      txBuff[0] = register_addr;
      r = I2C_MasterWriteBlocking(I2C1, txBuff, buffer_size, kI2C_TransferDefaultFlag);
      if(r)
      {
          /* If error occours, send STOP. */
          I2C_MasterStop(I2C1);
          return ;
      }
      if (kStatus_Success == I2C_MasterRepeatedStart(I2C1, I2C_Addr, kI2C_Read))
      {

        r = I2C_MasterReadBlocking(I2C1, data, size, 0);
        r = I2C_MasterStop(I2C1);
      }
    }
}

/*
 * Lectura de botones
 * Se implementa un sistema antirrebote en la función
 */

void lectura_boton (uint8_t puerto, uint8_t boton, estado_boton_t *estado){
	uint8_t lectura = GPIO_PinRead (GPIO, puerto, boton);

	switch (*estado){
		case soltado:
			if (lectura == 0){
				*estado = pulsado_antirrebote;
				flag_tick_boton = 0;
			}
			break;
		case pulsado_antirrebote:
			if (flag_tick_boton >= TIEMPO_ANTIRREBOTE){
				if (lectura == 0) *estado = pulsado;
				else *estado = soltado;
			}
			break;
		case pulsado:
			if (lectura == 1){
				*estado = soltado_antirrebote;
				flag_tick_boton = 0;
			}
			break;
		case soltado_antirrebote:
			if (flag_tick_boton >= TIEMPO_ANTIRREBOTE){
				if (lectura == 1) *estado = soltado;
				else *estado = pulsado;
			}
			break;
	}
}


/*
 * Inicialización USART
 * Se configura el puerto USART y se activa la interrupción
 */

void init_SWM_USART(USART_Type * port, uint8_t rx, uint8_t tx) {
	USART_Type * p_port;
	uint8_t p_tx;
	uint8_t p_rx;
	uint16_t p_clock;
	uint8_t p_nvic;
	switch((uintptr_t) port) {
		case USART0_BASE:
			p_port = USART0;
			p_clock = kUART0_Clk_From_MainClk;
			p_nvic = USART0_IRQn;
			p_tx = kSWM_USART0_TXD;
			p_rx = kSWM_USART0_RXD;
			break;
		case USART1_BASE:
			p_port = USART1;
			p_clock = kUART1_Clk_From_MainClk;
			p_nvic = USART1_IRQn;
			p_tx = kSWM_USART1_TXD;
			p_rx = kSWM_USART1_RXD;
			break;
		case USART2_BASE:
      		p_port = USART2;
			p_clock = kUART2_Clk_From_MainClk;
			p_nvic = USART2_IRQn;
			p_tx = kSWM_USART2_TXD;
			p_rx = kSWM_USART2_RXD;
			break;
		case USART3_BASE:
      		p_port = USART3;
			p_clock = kUART3_Clk_From_MainClk;
			p_nvic = PIN_INT6_USART3_IRQn;
			p_tx = kSWM_USART3_TXD;
			p_rx = kSWM_USART3_RXD;
			break;
		case USART4_BASE:
      		p_port = USART4;
			p_clock = kUART4_Clk_From_MainClk;
			p_nvic = PIN_INT7_USART4_IRQn;
			p_tx = kSWM_USART4_TXD;
			p_rx = kSWM_USART4_RXD;
			break;
		default:
			p_port = USART0;
			p_clock = kUART0_Clk_From_MainClk;
			p_nvic = USART0_IRQn;
			p_tx = kSWM_USART0_TXD;
			p_rx = kSWM_USART0_RXD;
			break;
	}
	CLOCK_EnableClock (kCLOCK_Swm);

	SWM_SetMovablePinSelect(SWM0, p_tx, tx);
	SWM_SetMovablePinSelect(SWM0, p_rx, rx);

	CLOCK_DisableClock(kCLOCK_Swm);

  // Setup config for usart - Corregir esos puntos  
	usart_config_t config;
	CLOCK_Select(p_clock);

	USART_GetDefaultConfig(&config);
	config.enableRx     = true;
	config.enableTx     = true;
	USART_Init(p_port, &config, CLOCK_GetFreq(kCLOCK_MainClk));

	USART_EnableInterrupts(p_port, kUSART_RxReadyInterruptEnable);
	NVIC_EnableIRQ(p_nvic);
}