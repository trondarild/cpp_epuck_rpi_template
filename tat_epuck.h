#include "tat_utils.h"


#define ACTUATORS_SIZE (19+1) // Data + checksum.
#define SENSORS_SIZE (46+1) // Data + checksum.
/**
 * Simple class for encapsulating epuck io
 */
class EPuck{
	
int fh;
uint8_t actuators_data[ACTUATORS_SIZE];
uint8_t sensors_data[SENSORS_SIZE];
uint16_t prox[8];
uint16_t prox_amb[8];
uint16_t mic[4];
uint8_t sel;
uint8_t button;
int16_t mot_steps[2];
uint8_t tv_remote;
uint8_t actuators_state = 0;

int update_robot_sensors_and_actuators();

public:

	EPuck();
	~EPuck();

	//state
	void setLED1(bool on);

	//update
	void tick();
};
