#include "def.h"

#define MOT_PORT_A1 0
#define MOT_PIN_A1  16
#define MOT_PORT_A2 0
#define MOT_PIN_A2  17
#define MOT_PORT_B1 0
#define MOT_PIN_B1  18
#define MOT_PORT_B2 0
#define MOT_PIN_B2  19

#define MOT_ANGLE_PER_STEP 15

struct step_seq_t {
    uint8_t mot_a1;
    uint8_t mot_a2;
    uint8_t mot_b1;
    uint8_t mot_b2;
};

extern struct step_seq_t sequence_full_steps[8];

void init_stepper();
void make_step(uint8_t step_state);