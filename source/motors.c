#include "motors.h"

// DC Motor
void init_dc_motor() {
    GPIO_PinInit(GPIO, MOTOR_PORT_1, MOTOR_PIN_1, &out_config_low);
    GPIO_PinInit(GPIO, MOTOR_PORT_2, MOTOR_PIN_2, &out_config_low);
}

void start_dc_motor(direction_t dir) {
    if (dir == CW) {
        GPIO_PinWrite(GPIO, MOTOR_PORT_1, MOTOR_PIN_1, 1);
        GPIO_PinWrite(GPIO, MOTOR_PORT_2, MOTOR_PIN_2, 0);
    } else {
        GPIO_PinWrite(GPIO, MOTOR_PORT_1, MOTOR_PIN_1, 0);
        GPIO_PinWrite(GPIO, MOTOR_PORT_2, MOTOR_PIN_2, 1);
    }
}

void stop_dc_motor() {
    GPIO_PinWrite(GPIO, MOTOR_PORT_1, MOTOR_PIN_1, 0);
    GPIO_PinWrite(GPIO, MOTOR_PORT_2, MOTOR_PIN_2, 0);
}

// Stepper Motor
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

struct step_seq_t sequence_half_steps[4] = {
    {1,0,0,0},
    {1,1,0,0},
    {0,1,0,0},
    {0,1,1,0}
};

void init_bipolar_stepper(direction_t dir) {
    GPIO_PinInit(GPIO, MOT_PORT_DIR, MOT_PIN_DIR, &out_config_low);
    GPIO_PinInit(GPIO, MOT_PORT_STEP, MOT_PIN_STEP, &out_config_low);
    GPIO_PinInit(GPIO, MOT_PORT_MSLEEP, MOT_PIN_MSLEEP, &out_config_low);
    GPIO_PinInit(GPIO, MOT_PORT_MRESET, MOT_PIN_MRESET, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_MS1, MOT_PIN_MS1, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_MS2, MOT_PIN_MS2, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_MS3, MOT_PIN_MS3, &out_config);
    set_bipolar_direction(dir);
}

void init_unipolar_stepper() {
    GPIO_PinInit(GPIO, MOT_PORT_A1, MOT_PIN_A1, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_A2, MOT_PIN_A2, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_B1, MOT_PIN_B1, &out_config);
    GPIO_PinInit(GPIO, MOT_PORT_B2, MOT_PIN_B2, &out_config);

}

void set_bipolar_direction(direction_t dir){
    GPIO_PinWrite(GPIO, MOT_PORT_DIR, MOT_PIN_DIR, dir);
}

void select_micro_steps(microstep_t option) {

    switch(option) {
        case full_step:
            GPIO_PinWrite(GPIO, MOT_PORT_MS1, MOT_PIN_MS1, 0);
            GPIO_PinWrite(GPIO, MOT_PORT_MS2, MOT_PIN_MS2, 0);
            GPIO_PinWrite(GPIO, MOT_PORT_MS3, MOT_PIN_MS3, 0);
            break;
        case half_step:
            GPIO_PinWrite(GPIO, MOT_PORT_MS1, MOT_PIN_MS1, 1);
            GPIO_PinWrite(GPIO, MOT_PORT_MS2, MOT_PIN_MS2, 0);
            GPIO_PinWrite(GPIO, MOT_PORT_MS3, MOT_PIN_MS3, 0);
            break;
        case quarter_step:
            GPIO_PinWrite(GPIO, MOT_PORT_MS1, MOT_PIN_MS1, 0);
            GPIO_PinWrite(GPIO, MOT_PORT_MS2, MOT_PIN_MS2, 1);
            GPIO_PinWrite(GPIO, MOT_PORT_MS3, MOT_PIN_MS3, 0);
            break;
        case eighth_step:
            GPIO_PinWrite(GPIO, MOT_PORT_MS1, MOT_PIN_MS1, 1);
            GPIO_PinWrite(GPIO, MOT_PORT_MS2, MOT_PIN_MS2, 1);
            GPIO_PinWrite(GPIO, MOT_PORT_MS3, MOT_PIN_MS3, 0);
            break;
        default:
            break;
    }
}

void make_bipolar_step(void) {
    // Make step
    GPIO_PinWrite(GPIO, MOT_PORT_STEP, MOT_PIN_STEP, 1);
    delay_mseg(MOT_STEP_TIME);
    GPIO_PinWrite(GPIO, MOT_PORT_STEP, MOT_PIN_STEP, 0);
    delay_mseg(MOT_STEP_TIME);

}

void make_unipolar_step(uint8_t step_state) {

    struct step_seq_t step = sequence_full_steps[step_state];

    GPIO_PinWrite(GPIO, MOT_PORT_A1, MOT_PIN_A1, step.mot_a1);
    GPIO_PinWrite(GPIO, MOT_PORT_A2, MOT_PIN_A2, step.mot_a2);
    GPIO_PinWrite(GPIO, MOT_PORT_B1, MOT_PIN_B1, step.mot_b1);
    GPIO_PinWrite(GPIO, MOT_PORT_B2, MOT_PIN_B2, step.mot_b2);

}

uint8_t move_bipolar_steps(uint16_t steps) {
    for (uint16_t i = 0; i < steps; i++)
        make_bipolar_step();
    return 0;

}
uint8_t move_bipolar_angle(uint16_t anglex10) {
    uint8_t steps = anglex10 * MOT_STEPS_PER_REV / 3600;
    if (steps < 1)
        return 1;

    for (uint8_t i = 0; i < steps; i++)
        make_bipolar_step();

    return 0;
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

void test_bipolar_stepper() {

    estado_boton_t usr_btn = soltado, isp_btn = soltado;
    uint8_t test_state = 0, test_mode = 0;
    uint8_t last_state = 0, last_step = 0, isp_push = 0, usr_push = 0;
    direction_t dir = CW;
    set_bipolar_direction(dir);
    start_stepper();
    while (1) {
        lectura_boton(0, USR_BTN, &usr_btn);
        lectura_boton(0, ISP_BTN, &isp_btn);

        if (usr_btn == pulsado && usr_push == 0) {
            test_state = !test_state;
            usr_btn = 1;
        }
        if (isp_btn == pulsado && isp_push == 0) {
            test_mode++;
            isp_push = 1;
        }
        if (isp_btn == soltado)
            isp_push = 0;
        if (usr_btn == soltado)
            usr_push = 0;
        if (test_mode >= 3) {
            test_mode = 0;
            start_stepper();
        }

        last_step = GPIO_PinRead(GPIO, MOT_PORT_STEP, MOT_PIN_STEP);
        if (test_mode == 0) {
            if (last_state != test_state) {
                move_bipolar_angle(MOT_ANGLE_PER_STEP * 10);
                last_state = test_state;
            }
        }
        else if (test_mode == 1) {
            make_bipolar_step();
        }
        else if (test_mode == 2) {
            stop_stepper();
            if (last_state != test_state) {
                if (dir == CW)
                    dir = CCW;
                else
                    dir = CW;
                set_bipolar_direction(dir);

                last_state = test_state;
            }
        }
        
        if (test_mode != 1)
            delay_mseg(100);
        else
            delay_mseg(25);
    }
}

void start_stepper() {
    GPIO_PinWrite(GPIO, MOT_PORT_MSLEEP, MOT_PIN_MSLEEP, 1);
}
void stop_stepper() {
    GPIO_PinWrite(GPIO, MOT_PORT_STEP, MOT_PIN_STEP, 0);
    GPIO_PinWrite(GPIO, MOT_PORT_MSLEEP, MOT_PIN_MSLEEP, 0);
}