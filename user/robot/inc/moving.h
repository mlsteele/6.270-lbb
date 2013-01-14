#ifndef _MOVING_H_
#define _MOVING_H_

#include <math.h>
#include <Point.h>

typedef struct {
	float r;
	float l;
} left_right_float_t;

left_right_float_t last_motor_pows = {0, 0};

#define SPEED 150
#define TARGET_TOLERANCE 150

float get_gyro_current_angle() {
	return fmod(gyro_get_degrees(), 360);
}

// TODO: do this
Point unimplemented_get_current_location() {
	Point p = {0,0};
	return p;
}

// void set_starting_loc() {
// 	//use gps
// 	current_loc.x = 0;
// 	current_loc.y = 0;
// }

// void update_location() {
// 	//use gps
// }

void set_velocity(float v) {
	motor_set_vel(PIN_MOTOR_DRIVE_L, v);
	motor_set_vel(PIN_MOTOR_DRIVE_R, v);
}

void set_wheel_pows(float l, float r) {
	motor_set_vel(PIN_MOTOR_DRIVE_L, l * 255);
	motor_set_vel(PIN_MOTOR_DRIVE_R, r * 255);
	last_motor_pows.l = l;
	last_motor_pows.r = r;
}

left_right_float_t get_wheel_pows() { return last_motor_pows; }

void wheels_brake() {
	set_wheel_pows(0, 0);
}

void decelerate(float millis) {
	//decelerates in the given amount of time
	//use jess's code
}

int robot_moving() {
	//need to call jess's methods
	return 0;
}

void rotate(float degrees) {
	printf("Rotating to a number of degrees");
	printf("start angle = %f \n", get_gyro_current_angle());
	float desired_angle = get_gyro_current_angle() + degrees;
	printf("desired_angle = %f \n", desired_angle);
	if (robot_moving()) {

	}
	else {
		if (degrees>0) {
			set_wheel_pows(0.58, -0.58);
			while (fmod(gyro_get_degrees(),360) < desired_angle) {
				printf("current angle = %f \n", get_gyro_current_angle());
			}
		}
		else {
			set_wheel_pows(-0.58, 0.58);
			while (fmod(gyro_get_degrees(),360) > desired_angle) {
				printf("current angle = %f \n", get_gyro_current_angle());
			}
		}
		wheels_brake();
	}
}

void move_to(Point p, float velocity) {
	//moves in a straight line to the desired point at the desired velocity
	float dist_x = p.x - unimplemented_get_current_location().x;
	float dist_y = p.y - unimplemented_get_current_location().y;
	float dist = sqrt(pow(dist_x, 2)+pow(dist_y, 2));
	float desired_angle = atan(dist_y/dist_x);

	rotate(desired_angle - get_gyro_current_angle());
	set_velocity(velocity);

// 	rotate(desired_angle-current_angle);
// 	set_velocity(velocity);
	while (dist > TARGET_TOLERANCE) {
		// update_location();
		dist_x = p.x - unimplemented_get_current_location().x;
		dist_y = p.y - unimplemented_get_current_location().y;
		dist = sqrt(pow(dist_x, 2)+pow(dist_y, 2));
		pause(100);
	}
	wheels_brake();
}

void move_for_time(float velocity, float millis) {
	printf("Moving for a certain amount of time");
	set_velocity(velocity);
	pause(millis);
	wheels_brake();
}

float get_heading() {
	//do some fancy averaging thing instead
	return gyro_get_degrees();
}

#endif
