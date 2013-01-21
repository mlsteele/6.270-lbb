#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <moving.h>
#include <math.h>
#include "../cat_mouse/get_vps.h"
#include "../cat_mouse/vps_utils.h"
#include "territory.h"

Point territories[6];

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  return 0;
}

int umain (void) {

	territories[0] = (Point) {0,    -858};
	territories[1] = (Point) {743,  -429};
	territories[2] = (Point) {743,   429};
	territories[3] = (Point) {0,     858};
	territories[4] = (Point) {-743,  429};
	territories[5] = (Point) {-743, -429};

	for(int i=0;i<12;i++){
		move_to_next_territory();
	}

	return 0;
}