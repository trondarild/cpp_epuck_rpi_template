//
//implementation EPuck
//
#include "tat_epuck.h"

#include <stdio.h>
#include <stdint.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/i2c-dev.h> /* for I2C_SLAVE */
//#include <linux/i2c.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define ROBOT_ADDR 0x1F
#define I2C_CHANNEL "/dev/i2c-12"
#define LEGACY_I2C_CHANNEL "/dev/i2c-4"


/** Constructur
 */
EPuck::EPuck(){

	uint16_t i = 0;
	uint8_t checksum = 0;
	struct timeval start_time, curr_time;	
	int32_t time_diff_us = 0;
	uint8_t counter = 0;

	// Set the I2C timeout to 20 ms (instead of 1 second). This need to be done on the "swticher" bus channel.
	int fh1 = open("/dev/i2c-1", O_RDWR);
	if(ioctl(fh1, I2C_TIMEOUT, 2) < 0) {
		perror("fail to set i2c1 timeout");
	}		
	close(fh1);

	fh = open(I2C_CHANNEL, O_RDWR);
	if(fh < 0) { // Try with bus number used in older kernel
		fh = open(LEGACY_I2C_CHANNEL, O_RDWR);	
		if(fh < 0) {
			perror("Cannot open I2C device");
			// return -1;
		}
	}

	ioctl(fh, I2C_SLAVE, ROBOT_ADDR);			// tell the driver we want the device with address 0x1F (7-bits) on the I2C bus

}

/** Destructor
 */
EPuck::~EPuck(){
}

void
EPuck::setLED1(FloatList &colors){
	// todo
}

void
EPuck::tick(){
	// todo assemble updates
	update_robot_sensors_and_actuators(); 

}


int 
EPuck::update_robot_sensors_and_actuators() {
	
    struct i2c_rdwr_ioctl_data packets;
    struct i2c_msg messages[2];
	int trials = 0;
	
	// S Addr Wr [A] Data(actuators) NA Sr Addr Rd [A] Data(sensors) [A] P
    messages[0].addr  = ROBOT_ADDR;
    messages[0].flags = 0;
    messages[0].len   = ACTUATORS_SIZE;
    messages[0].buf   = (char*)actuators_data;
	
    messages[1].addr  = ROBOT_ADDR;
    messages[1].flags = I2C_M_RD;
    messages[1].len   = SENSORS_SIZE;
    messages[1].buf   = (char*)sensors_data;

    packets.msgs      = messages;
    packets.nmsgs     = 2;
	
	// Form the tests it was noticed that sometimes (about 1/1000) the communication give a "timeout error" followed by "remote I/O" error.
	// Thus 3 retrials are done in case of errors.
	while(trials < 3) {
		if(ioctl(fh, I2C_RDWR, &packets) < 0) {		
			trials++;
			continue;
		}
		break;
	}

	if(trials > 2) {
		perror("update_robot_sensors_and_actuators: ");
		return -1;
	} else {
		return 0;
	}
}
