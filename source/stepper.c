#include "stepper.h"

struct step_seq_t sequence_full_steps[8];

void init_stepper() {
    gpio_pin_config_t out_config = {kGPIO_DigitalOutput, 0};
    gpio_pin_config_t in_config = {kGPIO_DigitalInput};

    GPIO_PortInit(GPIO, 0);
    GPIO_PortInit(GPIO, 1);
    GPIO_PinInit(GPIO, MOT_PORT_A1, MOT_PIN_A1, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_A2, MOT_PIN_A2, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_B1, MOT_PIN_B1, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_B2, MOT_PIN_B2, &out_config);

}

void make_step(uint8_t step_state) {

    struct step_seq_t step = sequence_full_steps[step_state];

    GPIO_PinWrite(GPIO, MOT_PORT_A1, MOT_PIN_A1, step.mot_a1);
    GPIO_PinWrite(GPIO, MOT_PORT_A2, MOT_PIN_A2, step.mot_a2);
    GPIO_PinWrite(GPIO, MOT_PORT_B1, MOT_PIN_B1, step.mot_b1);
    GPIO_PinWrite(GPIO, MOT_PORT_B2, MOT_PIN_B2, step.mot_b2);

}