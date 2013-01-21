#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <moving.h>
#include <math.h>
#include "../cat_mouse/get_vps.h"
#include "../cat_mouse/vps_utils.h"
#include "territory.h"

Point territories[6];

void move_to_territory(uint8_t territory){
	//Pick a path 
	int dir;
	if((territory-current_territory())%6 <= 3) 
		 {dir =  1;} 
	else {dir = -1;}

	//move into the target territory
	while(territory-current_territory() != 0){ 
		vps_aim_towards_target(territories[(current_territory()+dir)%6]);
	}
	//move to center of current territory
	while(find_distance(territories[target_territory])>200){
		vps_aim_towards_target(territories[current_territory()]);
	}
	set_wheel_pows(0,0);
}

void move_to_next_territory(){
	uint8_t target_territory = (current_territory()+1)%6;
	while(find_distance(territories[target_territory])>200){
		printf("\t%.2f away...\n", find_distance(territories[target_territory]));
		vps_aim_towards_target(territories[target_territory]);
	}
	set_wheel_pows(0,0);
}

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