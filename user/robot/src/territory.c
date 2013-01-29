#include <territory.h>
#include <hw_config.h>
#include <math.h>
#include <vps_data_daemon.h>
#include <transport.h>
#include <moving.h>
#include "../cat_mouse/vps_utils.h"
float terr_angles[7] = {-120,-60,0,60,120,180,240};
Point current_coords;
float current_theta;

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

	current_coords = (Point) {0, -858};
	current_theta = 90;
}

void territory_init() {
	printf("initializing territories... ");
	for (int i = 0; i < 6; i++) {
		float light_angle      = 60 * i;
		float gear_angle       = 13.6 + 60 * i;
		float lever_angle      = 360 + -13.54 + 60 * i;
		territory_angle_mid[i] = 60 * i;
		territory_angle_cw[i]  = 360 - 30 + 60 * i;
		territory_angle_ccw[i] = 30 + 60 * i;
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

void move_to_next_territory_dr(){

	Point target = territories[(current_territory() + 1) % 6];
	printf("Current -> (%.0f,%.0f)\n", current_coords.x, current_coords.y);
	printf("Target  -> (%.0f,%.0f)\n", target.x, target.y);

	float dtheta = atan2(target.y - current_coords.y, target.x - current_coords.x) / DEGS_TO_RADS - current_theta;
	rotate_by_gyro(dtheta);

	float distance = sqrt(pow(target.x-current_coords.x,2) + pow(target.y-current_coords.y,2));
	do {
		move_distance_by_encoders(distance);
		float distance = sqrt(pow(target.x-current_coords.x,2) + pow(target.y-current_coords.y,2));
		printf("\tDistance -> %.2f\n", distance);
	} while (distance > 200);
	
	/*while(points_distance(get_vps_position(), territories[target_territory]) > 200){
		printf("%.2f away... from target %.2f, %.2f\n", points_distance(get_vps_position(), territories[target_territory]), territories[target_territory].x, territories[target_territory].y);
		vps_aim_towards_target(territories[target_territory], 1);
		pause(10);
	}*/
}

void move_to_territory_dr(uint8_t terr){

	Point target = territories[terr % 6];
	printf("Current -> (%.0f,%.0f)\n", current_coords.x, current_coords.y);
	printf("Target  -> (%.0f,%.0f)\n", target.x, target.y);

	float dtheta = atan2(target.y - current_coords.y, target.x - current_coords.x) / DEGS_TO_RADS - current_theta;
	printf("Current Theta -> %.0f\n", current_theta);
	printf("Delta Theta   -> %.0f\n", dtheta);
	rotate_by_gyro(dtheta);

	float distance = sqrt(pow(target.x-current_coords.x,2) + pow(target.y-current_coords.y,2));
	printf("Distance -> %.0f\n", distance);
	move_distance_by_encoders(distance);
	
	current_coords = target;
	current_theta += dtheta;
	/*while(points_distance(get_vps_position(), territories[target_territory]) > 200){
		printf("%.2f away... from target %.2f, %.2f\n", points_distance(get_vps_position(), territories[target_territory]), territories[target_territory].x, territories[target_territory].y);
		vps_aim_towards_target(territories[target_territory], 1);
		pause(10);
	}*/
}

void move_to_gear() {
	//These numbers will have to be tweaked
	//maybe we don't even need the loop
	while(points_distance(get_vps_position(),gears[current_territory()]) > 200){
		vps_aim_towards_target(gears[current_territory()], 1);
		pause(20);
	}
	set_wheel_pows(0,0);
}

void spin_gear() {
  //motor_set_vel(PIN_MOTOR_DRIVE_L, l > -255 ? (l < 255 ? (l * 255) : 255) : -255);

}