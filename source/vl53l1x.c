#include "vl53l1x.h"


void init_vl53l1x(uint16_t dev, uint16_t mode) {
    uint8_t sensorState=0, status;
    // Test de valores en lectura
    // status = VL53L1_RdByte(dev, 0x010F, &byteData);
    // PRINTF("VL53L1X Model_ID: %X\n", byteData);

    // status = VL53L1_RdByte(dev, 0x0110, &byteData);
    // printf("VL53L1X Module_Type: %X\n", byteData);
    
    // status = VL53L1_RdByte(dev, 0x0111, &byteData);
    // printf("VL53L1X Module_Type: %X\n", byteData);

    // status = VL53L1_RdWord(dev, 0x010F, &wordData);
    // printf("VL53L1X: %X\n", wordData);

    while(sensorState==0){
      status = VL53L1X_BootState(dev, &sensorState);
      delay_mseg(200);
    }
    printf("Chip booted\n");

    /* This function must to be called to initialize the sensor with the default setting  */
    status = VL53L1X_SensorInit(dev);
    /* Optional functions to be used to change the main ranging parameters according the application requirements to get the best ranging performances */
    status = VL53L1X_SetDistanceMode(dev, mode); /* 1=short, 2=long */
    status = VL53L1X_SetTimingBudgetInMs(dev, 100); /* in ms possible values [20, 50, 100, 200, 500] */
    status = VL53L1X_SetInterMeasurementInMs(dev, 100); /* in ms, IM must be > = TB */
    status = VL53L1X_StartRanging(dev);   /* This function has to be called to enable the ranging */

}


laser_data_t get_data_laser(uint16_t dev) {
    uint8_t status;

    laser_data_t data;
    status = VL53L1X_CheckForDataReady(dev, &data.ready);
    if (data.ready != 0) {
        status = VL53L1X_GetRangeStatus(dev, &data.range);
        status = VL53L1X_GetDistance(dev, &data.distance);
        status = VL53L1X_GetSignalRate(dev, &data.signal_rate);
        status = VL53L1X_GetAmbientRate(dev, &data.ambient_light);
        status = VL53L1X_GetSpadNb(dev, &data.spad_num);
        /* clear interrupt has to be called to enable next interrupt*/
        status = VL53L1X_ClearInterrupt(dev);
    }
    return data;
}

uint8_t send_laser_uart(laser_data_t data, USART_Type *usart_port, uint16_t mot_angle) {
    char msg_usart[128] = "";
    if (data.ready != 0){
        sprintf(msg_usart, "$%u, %u, %u, %u, %u, %u\n",
         data.range,
         data.distance,
         data.signal_rate,
         data.ambient_light,
         data.spad_num,
         mot_angle % 3600
        );
        USART_WriteBlocking(usart_port, msg_usart, strlen(msg_usart));
    } else {
        sprintf(msg_usart, "Data not ready %i\n", data.ready);
        USART_WriteBlocking(usart_port, msg_usart, strlen(msg_usart));
    }
}