
#define ENCODER_PIN 25
#define LSB_US_PER_DEG 1386583
#define GRYO 21
#define MOTOR_L 0
#define MOTOR_R 1
#define DRIVE_TOLERANCE 150

// Include headers from OS
#include <joyos.h>
#include "Point.h"
#include "encoder_utils.h"
#include "moving.h"
#include "vps_utils.h"

// usetup is called during the calibration period. 
int usetup (void) {
	extern volatile uint8_t robot_id;
  	robot_id = 8;
  	printf("Stabilizing\n");
	pause(500);
	printf("Initiating gyro\n");
	gyro_init(GRYO, LSB_US_PER_DEG, 500);
	printf("Gryoscope initated\n");
	set_starting_loc(); 
    return 0;
}

// Entry point to contestant code.
int umain (void) {
	
    return 0;
}
