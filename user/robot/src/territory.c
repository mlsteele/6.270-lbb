#include <territory.h>
#include <hw_config.h>
#include <math.h>
#include <vps_data_daemon.h>
#include <transport.h>
#include <moving.h>
#include "../cat_mouse/vps_utils.h"

float terr_angles[7] = {-120,-60,0,60,120,180,240};

void territory_init() {
	for (int i = 0; i < 6; i++) {
		float light_angle = 180 + 90  + 0     + 60 * i;
		float gear_angle  = 180 + 180 - 76.16 + 60 * i;
		float lever_angle = 180 + 0   + 76.55 + 60 * i;
		territories[i] = (Point) {
			TERRITORY_RAD_TO_LIGHT * cos(light_angle * DEGS_TO_RADS) ,
			TERRITORY_RAD_TO_LIGHT * sin(light_angle * DEGS_TO_RADS) };
		gears[i] = (Point) {
			TERRITORY_RAD_TO_GEARS * cos(gear_angle * DEGS_TO_RADS) ,
			TERRITORY_RAD_TO_GEARS * sin(gear_angle * DEGS_TO_RADS) };
		mines[i] = (Point) {
			TERRITORY_RAD_TO_MINE * cos(lever_angle * DEGS_TO_RADS) ,
			TERRITORY_RAD_TO_MINE * sin(lever_angle * DEGS_TO_RADS) };
	}

	for (int i = 0; i < 6; i++) {
		printf("territories[%i] -> %f, %f\n", i, territories[i].x, territories[i].y);
	}
	for (int i = 0; i < 6; i++) {
		printf("gears[%i] -> %f, %f\n", i, gears[i].x, gears[i].y);
	}
	for (int i = 0; i < 6; i++) {
		printf("mines[%i] -> %f, %f\n", i, mines[i].x, mines[i].y);
	}

	//= (Point) {TERRITORY_RAD_TO_LIGHT * cos(angle * DEGS_TO_RADS), TERRITORY_RAD_TO_LIGHT * sin(angle * DEGS_TO_RADS)};
	/*territories[0] = (Point){0,    -858};
	territories[1] = (Point){743,  -429};
	territories[2] = (Point){743,   429}; 
	territories[3] = (Point){0,     858}; 
	territories[4] = (Point){-743,  429}; 
	territories[5] = (Point){-743, -429};*/

	// gears[0] = (Point){297.2, -1206.4};
	// gears[1] = (Point){1193.4,   -346};
	// gears[2] = (Point){896.3,   860.5};
	// gears[3] = (Point){-297.2, 1206.4};
	// gears[4] = (Point){-1193.4,   346};
	// gears[5] = (Point){-896.3, -860.5};

	// mines[0] = (Point){-291, -1215}; 
	// mines[1] = (Point){907,   -859}; 
	// mines[2] = (Point){1197,   356}; 
	// mines[3] = (Point){291,   1215};
	// mines[4] = (Point){-907,   859}; 
	// mines[5] = (Point){-1197, -356}; 

}

void territory_check() {
	printf("territory initialization check\n");
	for (int i = 0; i < 6; i++) {
		printf("territories[%i] -> %f, %f\n", i, territories[i].x, territories[i].y);
	}
	for (int i = 0; i < 6; i++) {
		printf("gears[%i] -> %f, %f\n", i, gears[i].x, gears[i].y);
	}
	for (int i = 0; i < 6; i++) {
		printf("mines[%i] -> %f, %f\n", i, mines[i].x, mines[i].y);
	}
}

int8_t territory_of_point(Point p) {
	float ang = atan2(p.y, p.x) / DEGS_TO_RADS;
	for(int i=0;i<6;i++){
		if((ang>terr_angles[i] && ang<terr_angles[i+1])
		|| (ang+360>terr_angles[i] && ang+360<terr_angles[i+1]))  {
			return i;
		}	
	}
	return 255;
}

int8_t current_territory() {
	return territory_of_point(get_vps_position());
}

void move_to_territory(int8_t territory){
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
	printf("sample territory %.2f, %.2f\n", territories[0].x, territories[0].y);

	int8_t this_territory = current_territory();
	int8_t target_territory = (this_territory + 5) % 6;
	printf("this_territory -> %i\n", this_territory);
	printf("target_territory -> %i\n", target_territory);

	while(points_distance(get_vps_position(), territories[target_territory]) > 200){
		printf("%.2f away... from target %.2f, %.2f\n", points_distance(get_vps_position(), territories[target_territory]), territories[target_territory].x, territories[target_territory].y);
		vps_aim_towards_target(territories[target_territory], 1);
		pause(10);
	}
	set_wheel_pows(0,0);
}
