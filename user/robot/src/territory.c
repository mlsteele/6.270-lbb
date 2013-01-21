#include <territory.h>
#include <hw_config.h>
#include <math.h>
#include <vps_data_daemon.h>
#include <transport.h>
#include <moving.h>
float terr_angles[7] = {-120,-60,0,60,120,180,240};

void territory_init() {
	territories[0] = (Point){0,    -858};
	territories[1] = (Point){743,  -429};
	territories[2] = (Point){743,   429}; 
	territories[3] = (Point){0,     858}; 
	territories[4] = (Point){-743,  429}; 
	territories[5] = (Point){-743, -429};

	gears[0] = (Point){297.2, -1206.4};
	gears[1] = (Point){1193.4,   -346};
	gears[2] = (Point){896.3,   860.5};
	gears[3] = (Point){-297.2, 1206.4};
	gears[4] = (Point){-1193.4,   346};
	gears[5] = (Point){-896.3, -860.5};

	mines[0] = (Point){-291, -1215}; 
	mines[1] = (Point){907,   -859}; 
	mines[2] = (Point){1197,   356}; 
	mines[3] = (Point){291,   1215};
	mines[4] = (Point){-907,   859}; 
	mines[5] = (Point){-1197, -356}; 

}
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
		vps_aim_towards_target(territories[(current_territory()+dir)%6], 1);
	}
	//move to center of current territory
	while(points_distance(get_vps_position(), territories[territory])>200){
		vps_aim_towards_target(territories[current_territory()], 1);
		pause(10);
	}
	set_wheel_pows(0,0);
}

void move_to_next_territory(){
	uint8_t target_territory = (current_territory()+1)%6;
	while(points_distance(get_vps_position(), territories[target_territory])>200){
		printf("\t%.2f away...\n", points_distance(get_vps_position(), territories[target_territory]));
		vps_aim_towards_target(territories[target_territory], 1);
		pause(10);
	}
	set_wheel_pows(0,0);
}