#include "stepper.h"
// https://i.sstatic.net/nyeOe.gif
struct step_seq_t sequence_full_steps[8] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {1, 0, 0, 0},
    {0, 1, 0, 0}
};
// struct step_seq_t sequence_full_steps[8] = {
//     {1, 0, 0, 1},
//     {1, 1, 0, 0},
//     {0, 1, 1, 0},
//     {0, 0, 1, 1},
//     {1, 0, 0, 1},
//     {1, 1, 0, 0},
//     {0, 1, 1, 0},
//     {0, 0, 1, 1}
// };

struct step_seq_t sequence_half_steps[4] = {
    {1,0,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,1,1,0}
};

void init_bipolar_stepper(direction_t dir) {
    GPIO_PinInit(GPIO, MOT_PORT_DIR, MOT_PIN_DIR, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_STEP, MOT_PIN_STEP, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_MS1, MOT_PIN_MS1, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_MS2, MOT_PIN_MS2, &out_config);
    set_bipolar_direction(dir);
}

void init_unipolar_stepper() {
    GPIO_PinInit(GPIO, MOT_PORT_A1, MOT_PIN_A1, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_A2, MOT_PIN_A2, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_B1, MOT_PIN_B1, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_B2, MOT_PIN_B2, &out_config);

}

void set_bipolar_direction(direction_t dir){
    // Set direction
    GPIO_PinWrite(GPIO, MOT_PORT_DIR, MOT_PIN_DIR, dir);
}

void select_micro_steps(microstep_t option) {

    switch(option) {
        case full_step:
            GPIO_PinWrite(GPIO, MOT_PORT_MS1, MOT_PIN_MS1, 0);
            GPIO_PinWrite(GPIO, MOT_PORT_MS2, MOT_PIN_MS2, 0);
            break;
        case half_step:
            GPIO_PinWrite(GPIO, MOT_PORT_MS1, MOT_PIN_MS1, 1);
            GPIO_PinWrite(GPIO, MOT_PORT_MS2, MOT_PIN_MS2, 0);
            break;
        case quarter_step:
            GPIO_PinWrite(GPIO, MOT_PORT_MS1, MOT_PIN_MS1, 0);
            GPIO_PinWrite(GPIO, MOT_PORT_MS2, MOT_PIN_MS2, 1);
            break;
        case eighth_step:
            GPIO_PinWrite(GPIO, MOT_PORT_MS1, MOT_PIN_MS1, 1);
            GPIO_PinWrite(GPIO, MOT_PORT_MS2, MOT_PIN_MS2, 1);
            break;
        default:
            break;
    }
}

void make_bipolar_step(void) {
    // Make step
    GPIO_PinWrite(GPIO, MOT_PORT_STEP, MOT_PIN_STEP, 1);
    delay_mseg(25);
    GPIO_PinWrite(GPIO, MOT_PORT_STEP, MOT_PIN_STEP, 0);

}

void make_unipolar_step(uint8_t step_state) {

    struct step_seq_t step = sequence_full_steps[step_state];

    GPIO_PinWrite(GPIO, MOT_PORT_A1, MOT_PIN_A1, step.mot_a1);
    GPIO_PinWrite(GPIO, MOT_PORT_A2, MOT_PIN_A2, step.mot_a2);
    GPIO_PinWrite(GPIO, MOT_PORT_B1, MOT_PIN_B1, step.mot_b1);
    GPIO_PinWrite(GPIO, MOT_PORT_B2, MOT_PIN_B2, step.mot_b2);

}

void test_unipolar_stepper() {
    estado_boton_t stop_test = soltado;
    int i = 0;
    while(stop_test != pulsado) {
        lectura_boton(0, USR_BTN, &stop_test);
        delay_mseg(500);
        make_unipolar_step(i);
        i++;
    }
}