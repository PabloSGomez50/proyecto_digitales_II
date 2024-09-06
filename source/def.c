#include "def.h"

void delay_mseg(int time) {

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