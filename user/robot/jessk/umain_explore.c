#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <moving.h>
#include <math.h>
#include "../cat_mouse/get_vps.h"
#include "../cat_mouse/vps_utils.h"


int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  return 0;
}

int umain (void) {

	Point territories[6];
	territories[0] = (Point) {0,    -858};
	territories[1] = (Point) {743,  -429};
	territories[2] = (Point) {743,   429};
	territories[3] = (Point) {0,     858};
	territories[4] = (Point) {-743,  429};
	territories[5] = (Point) {-743, -429};

	for(int i=0; i<6; i++){
		printf("Moving to target %d (%.2f, %.2f)\n", i, territories[i].x, territories[i].y);
		while(find_distance(territories[i])>200){
			printf("\t%.2f away...\n", find_distance(territories[i]));
			aim_towards_target_vps(territories[i]);
		}
	}
	set_wheel_pows(0,0);

	return 0;
}