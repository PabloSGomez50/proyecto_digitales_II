#ifndef MOTORS_H_
#define MOTORS_H_

#include "def.h"


typedef enum {
    CW = 0, 
    CCW = 1
} direction_t;

typedef enum {
    full_step,
    half_step,
    quarter_step,
    eighth_step
} microstep_t;

struct step_seq_t {
    uint8_t mot_a1 : 1;
    uint8_t mot_a2 : 1;
    uint8_t mot_b1 : 1;
    uint8_t mot_b2 : 1;
};


// DC Motor
#define MOTOR_PORT_1    0
#define MOTOR_PIN_1     8
#define MOTOR_PORT_2    0
#define MOTOR_PIN_2     9

void init_dc_motor(void);
void start_dc_motor(direction_t dir);
void stop_dc_motor(void);

// Steppers

#define MOT_PORT_A1 0
#define MOT_PIN_A1  10
#define MOT_PORT_A2 0
#define MOT_PIN_A2  9
#define MOT_PORT_B1 1
#define MOT_PIN_B1  2
#define MOT_PORT_B2 0
#define MOT_PIN_B2  15

#define MOT_STEPS_PER_REV 200
#define MOT_ANGLE_PER_STEP 3600 / MOT_STEPS_PER_REV
#define MOT_STEP_TIME 5
#define MOT_RATIO 2

#define MOT_PORT_STEP   0
#define MOT_PIN_STEP    8
#define MOT_PORT_DIR    0
#define MOT_PIN_DIR     9
#define MOT_PORT_MS1    1
#define MOT_PIN_MS1     2
#define MOT_PORT_MS2    0
#define MOT_PIN_MS2     15
#define MOT_PORT_MS3    0
#define MOT_PIN_MS3     14
#define MOT_PORT_MRESET 0
#define MOT_PIN_MRESET  13
#define MOT_PORT_MSLEEP 0
#define MOT_PIN_MSLEEP  11



extern struct step_seq_t sequence_full_steps[8];
extern struct step_seq_t sequence_half_steps[4];

void init_unipolar_stepper();
void init_bipolar_stepper(direction_t dir);

void test_unipolar_stepper();
void test_bipolar_stepper();

void make_unipolar_step(uint8_t step_state);

void select_micro_steps(microstep_t option);
void set_bipolar_direction(direction_t dir);
void make_bipolar_step(void);
uint8_t move_bipolar_angle(uint16_t anglex10);
uint8_t move_bipolar_steps(uint16_t steps);
void start_stepper();
void stop_stepper();

#endif