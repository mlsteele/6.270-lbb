#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <moving.h>
#include <math.h>
#include "get_vps.h"

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  return 0;
}

int umain (void) {

	// THE HIGHEST LEVEL OF THOUGHT PROCESSES
	// NOW AVAILABLE IN GLORIOUS, GLORIOUS PSEUDOCODE

	while(time_elapsed()<10000){
		//first 10 seconds, explore only
		explore();
	}

	while(1) {
		//Decide next move
		if(numb_balls() >= max_balls() || time_left() > 20000) {
			//if you are full of balls or time is about to run out (20sec)
			move_to(base);
			drop_balls();
		}
	}

    return 0;
}

