#ifndef MOVING_H
#define MOVING_H
#include <math.h>

struct Point current_loc;
float current_angle = 0; //probably want to set it to a different value

void set_starting_loc(void) {
	//use gps
	current_loc.x = 0;
	current_loc.y = 0;
}

void update_location() {
	//use gps
}

void set_velocity(float v) {
	motor_set_vel(MOTOR_L, v);
	motor_set_vel(MOTOR_R, v);
}

void set_velocities(float l, float r) {
	motor_set_vel(MOTOR_L, l);
	motor_set_vel(MOTOR_R, r);
}

void brake(void) {
//	motor_brake(MOTOR_R);
//	motor_brake(MOTOR_L);
	motor_set_vel(MOTOR_R ,0);
	motor_set_vel(MOTOR_L, 0);
}

void deccelerate(float millis) {
	//deccelerates in the given amount of time
	//use jess's code
}

int robot_moving(void) {
	//need to call jess's methods
	return 0;
}

void rotate(float degrees) {
	printf("Rotating to a number of degrees");
	current_angle = fmod(gyro_get_degrees(),360);
	printf("start angle = %f \n", current_angle);
	float desired_angle = current_angle + degrees;
	printf("desired_angle = %f \n", desired_angle);
	if (robot_moving()) {

	}
	else {
		if (degrees>0) {
			set_velocities(150, -150);
			while (fmod(gyro_get_degrees(),360) < desired_angle) {
				printf("current angle = %f \n", fmod(gyro_get_degrees(),360));
			}
		}
		else {
			set_velocities(-150, 150);
			while (fmod(gyro_get_degrees(),360) > desired_angle) {
				printf("current angle = %f \n", fmod(gyro_get_degrees(),360));
			}
		}
		brake();
	}
	current_angle = gyro_get_degrees();
}

void move_to(struct Point p, float velocity) {
	//moves in a straight line to the desired point at the desired velocity
	current_angle = gyro_get_degrees();
	float dist_x = p.x - current_loc.x;
	float dist_y = p.y - current_loc.y;
	float dist = sqrt(pow(dist_x, 2)+pow(dist_y, 2));
	float desired_angle = atan(dist_y/dist_x);
	
	rotate(desired_angle-current_angle);
	set_velocity(velocity);

	while (dist > DRIVE_TOLERANCE) {
		update_location();
		dist_x = p.x - current_loc.x;
		dist_y = p.y - current_loc.y;
		dist = sqrt(pow(dist_x, 2)+pow(dist_y, 2));
		pause(100);
	}
	brake();
}

void move_for_time(float velocity, float millis) {
	printf("Moving for a certain amount of time");
	set_velocity(velocity);
	pause(millis);
	brake();
}
#ENDIF