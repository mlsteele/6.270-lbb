// Include headers from OS
#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <moving.h>

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
