#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <moving.h>
#include <math.h>
#include "../cat_mouse/vps_utils.h"


int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  return 0;
}

int umain (void) {

	Point territories[6];
	territories[0] = {0,    -858};
	territories[1] = {743,  -429};
	territories[2] = {743,   429};
	territories[3] = {0,     858};
	territories[4] = {-743,  429};
	territories[5] = {-743, -429};

	for(int i=0; i<6; i++){
		printf("Moving to target %d (%.2f, %.2f)\n", i, i.x, i.y);
		while(find_distance(territories[i])<200){
			printf("\t%.2f away...\n", find_distance(territories[i]));
			vps_aim_towards_target(territories[i]);
		}
	}
}