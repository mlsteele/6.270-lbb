#include <territory.h>
#include <hw_config.h>
#include <math.h>
#include <vps_data_daemon.h>
#include <transport.h>
#include <moving.h>
#include "../cat_mouse/vps_utils.h"

static void territory_check() {
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

void territory_init() {
	printf("initializing territories... ");
	for (int i = 0; i < 6; i++) {
		float light_angle      = 60 * i;
		float gear_angle       = 8.8 + 60 * i;
		float lever_angle      = 360 - 20 + 60 * i;
		territory_angle_mid[i] = 60 * i;
		territory_angle_cw[i]  = 360 - 30 + 60 * i;
		territory_angle_ccw[i] = 30 + 60 * i;

		territories[i] = (Point) {
			TERRITORY_RAD_TO_LIGHT * cos(light_angle * DEGS_TO_RADS) ,
			TERRITORY_RAD_TO_LIGHT * sin(light_angle * DEGS_TO_RADS) };

		gears[i] = (Point) {
			TERRITORY_RAD_TO_GEARS * cos(gear_angle * DEGS_TO_RADS) ,
			TERRITORY_RAD_TO_GEARS * sin(gear_angle * DEGS_TO_RADS) };

		pre_gears[i] = (Point) {
			TERRITORY_RAD_TO_PRE_GEARS * cos(gear_angle * DEGS_TO_RADS) ,
			TERRITORY_RAD_TO_PRE_GEARS * sin(gear_angle * DEGS_TO_RADS) };

		mines[i] = (Point) {
			TERRITORY_RAD_TO_MINE * cos(lever_angle * DEGS_TO_RADS) ,
			TERRITORY_RAD_TO_MINE * sin(lever_angle * DEGS_TO_RADS) };

		float pre_mine_angle = light_angle - 30 + 180;
		float pre_mine_displacement = 400;
		pre_mines[i] = (Point) {
			mines[i].x + cos(pre_mine_angle * DEGS_TO_RADS) * pre_mine_displacement ,
			mines[i].y + sin(pre_mine_angle * DEGS_TO_RADS) * pre_mine_displacement };
		
		pre_mines[i] = (Point) {
			TERRITORY_RAD_TO_LIGHT * cos(lever_angle * DEGS_TO_RADS) ,
			TERRITORY_RAD_TO_LIGHT * sin(lever_angle * DEGS_TO_RADS) };

	}
	printf("done\n");

	territory_check();
}

int8_t territory_of_point(Point p) {
	float ang = atan2(p.y, p.x) * RADS_TO_DEGS;
	for (int i = 0; i < 6; i++) {
		if (fabs(ang_diff(ang, territory_angle_mid[i])) < 30) {
			return i;
		}
	}

	printf("WARN: territory_of_point (%.5f, %.5f) failed\n", p.x, p.y);
	return -1;
}

int8_t current_territory() {
	return territory_of_point(get_vps_position());
}

// void move_to_territory(int8_t territory){
// 	//Pick a path 
// 	//TODO include enemy in calculations of path
// 	int dir;
// 	if((territory-current_territory())%6 <= 3) 
// 		 {dir =  1;} 
// 	else {dir = -1;}

// 	//move into the target territory
// 	while(territory-current_territory() != 0){ 
// 		aim_towards_target_vps(territories[(current_territory()+dir)%6]);
// 	}
// 	//move to center of current territory
// 	while(points_distance(get_vps_position(), territories[territory])>200){
// 		aim_towards_target_vps(territories[current_territory()]);
// 		pause(10);
// 	}
// 	set_wheel_pows(0,0);
// }

// void move_to_next_territory(){
// 	printf("sample territory %.2f, %.2f\n", territories[0].x, territories[0].y);

// 	int8_t this_territory = current_territory();
// 	int8_t target_territory = (this_territory + 5) % 6;
// 	printf("this_territory -> %i\n", this_territory);
// 	printf("target_territory -> %i\n", target_territory);

// 	while(points_distance(get_vps_position(), territories[target_territory]) > 200){
// 		printf("%.2f away... from target %.2f, %.2f\n", points_distance(get_vps_position(), territories[target_territory]), territories[target_territory].x, territories[target_territory].y);
// 		aim_towards_target_vps(territories[target_territory]);
// 		pause(10);
// 	}
// 	set_wheel_pows(0,0);
// }
