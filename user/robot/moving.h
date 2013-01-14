#ifndef _MOVING_H_
#define _MOVING_H_
#include <math.h>

#define SPEED 150
#define TARGET_TOLERANCE 150

Point current_loc;
float current_angle = 0; //probably want to set it to a different value

void set_starting_loc(void) {
	//use gps
	current_loc.x = 0;
	current_loc.y = 0;
}

// void update_location() {
// 	//use gps
// }

void set_velocity(float v) {
	motor_set_vel(PIN_MOTOR_DRIVE_L, v);
	motor_set_vel(PIN_MOTOR_DRIVE_R, v);
}

void set_velocities(float l, float r) {
	motor_set_vel(PIN_MOTOR_DRIVE_L, l);
	motor_set_vel(PIN_MOTOR_DRIVE_R, r);
}

void brake(void) {
//	motor_brake(PIN_MOTOR_DRIVE_R);
//	motor_brake(PIN_MOTOR_DRIVE_L);
	motor_set_vel(PIN_MOTOR_DRIVE_R ,0);
	motor_set_vel(PIN_MOTOR_DRIVE_L, 0);
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
	current_angle = fmod(get_heading(),360);
	printf("start angle = %f \n", current_angle);
	float desired_angle = current_angle + degrees;
	printf("desired_angle = %f \n", desired_angle);
	if (robot_moving()) {

	}
	else {
		if (degrees>0) {
			set_velocities(SPEED, -SPEED);
			while (fmod(get_heading(),360) < desired_angle) {
				printf("current angle = %f \n", fmod(get_heading(),360));
			}
		}
		else {
			set_velocities(-SPEED, SPEED);
			while (fmod(get_heading(),360) > desired_angle) {
				printf("current angle = %f \n", fmod(get_heading(),360));
			}
		}
		brake();
	}
}

void move_to(Point p, float velocity) {
	//moves in a straight line to the desired point at the desired velocity
	current_angle = gyro_get_degrees();
	float dist_x = p.x - current_loc.x;
	float dist_y = p.y - current_loc.y;
	float dist = sqrt(pow(dist_x, 2)+pow(dist_y, 2));
	float desired_angle = atan(dist_y/dist_x);

// 	rotate(desired_angle-current_angle);
// 	set_velocity(velocity);
	while (dist > TARGET_TOLERANCE) {
		// update_location();
		dist_x = p.x - vps_position.x;
		dist_y = p.y - vps_position.y;
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

float get_heading() {
	//do some fancy averaging thing instead
	return gyro_get_degrees();
}

#endif
