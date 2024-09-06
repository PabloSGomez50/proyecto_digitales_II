#include "VL53L1X_api.h"
#include "def.h"

#define MEASUREMENT_BUDGET_MS 50
#define INTER_MEASUREMENT_PERIOD_MS 55
#define VL53L1X_ADDRESS	 			0x52
 /* 1=short, 2=long */

typedef enum {
    short_distance = 1,
    long_distance = 2,
}laser_dist_mode;

struct laser_data_t {
    uint8_t ready;
    uint16_t distance;
    uint8_t range;
    uint16_t signal_rate;
    uint16_t ambient_light;
    uint16_t spad_num;

};

extern uint16_t dev;


void init_vl53l1x(uint16_t dev, uint16_t modo);
struct laser_data_t get_data_laser(uint16_t dev);