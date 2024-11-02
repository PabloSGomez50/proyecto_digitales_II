#include "def.h"

#define MOT_PORT_A1 0
#define MOT_PIN_A1  20
#define MOT_PORT_A2 0
#define MOT_PIN_A2  21
#define MOT_PORT_B1 0
#define MOT_PIN_B1  22
#define MOT_PORT_B2 0
#define MOT_PIN_B2  23

#define MOT_ANGLE_PER_STEP 15

#define MOT_PORT_DIR    MOT_PORT_A1
#define MOT_PIN_DIR     MOT_PIN_A1
#define MOT_PORT_STEP   MOT_PORT_A2
#define MOT_PIN_STEP    MOT_PIN_A2
#define MOT_PORT_MS1    MOT_PORT_B1
#define MOT_PIN_MS1     MOT_PIN_B1
#define MOT_PORT_MS2    MOT_PORT_B2
#define MOT_PIN_MS2     MOT_PIN_B2

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

extern struct step_seq_t sequence_full_steps[8];
extern struct step_seq_t sequence_half_steps[4];

void init_unipolar_stepper();
void init_bipolar_stepper(direction_t dir);

void test_unipolar_stepper();

void make_unipolar_step(uint8_t step_state);

void select_micro_steps(microstep_t option);
void set_bipolar_direction(direction_t dir);
void make_bipolar_step(void);