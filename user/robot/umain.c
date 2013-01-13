
#define ENCODER_PIN 25

// Include headers from OS
#include <joyos.h>
#include <encoder_utils.h>
#include <vps_utils.h>

// usetup is called during the calibration period. 
int usetup (void) {
	extern volatile uint8_t robot_id;
  	robot_id = 8;
    return 0;
}

// Entry point to contestant code.
int umain (void) {
	
    return 0;
}
