#include <territory.h>
#include <hw_config.h>
#include <math.h>
#include <vps_data_daemon.h>
float terr_angles[7] = {-120,-60,0,60,120,180,240};
Point territories[6];

uint8_t territory_of_point(Point p) {
	float ang = atan2(p.y, p.x) / DEGS_TO_RADS;
	for(int i=0;i<6;i++){
		if((ang>terr_angles[i] && ang<terr_angles[i+1])
		|| (ang+360>terr_angles[i] && ang+360<terr_angles[i+1]))  {
			return i;
		}	
	}
	return 255;
}
uint8_t current_territory() {
	return territory_of_point(get_vps_position());
}

void move_to_territory(uint8_t territory){
	//Pick a path 
	//TODO include enemy in calculations of path
	int dir;
	if((territory-current_territory())%6 <= 3) 
		 {dir =  1;} 
	else {dir = -1;}

	//move into the target territory
	while(territory-current_territory() != 0){ 
		vps_aim_towards_target(territories[(current_territory()+dir)%6]);
	}
	//move to center of current territory
	while(find_distance(territories[territory])>200){
		vps_aim_towards_target(territories[current_territory()]);
		pause(10);
	}
	set_wheel_pows(0,0);
}

void move_to_next_territory(){
	uint8_t target_territory = (current_territory()+1)%6;
	while(find_distance(territories[target_territory])>200){
		printf("\t%.2f away...\n", find_distance(territories[target_territory]));
		vps_aim_towards_target(territories[target_territory]);
		pause(10);
	}
	set_wheel_pows(0,0);
}