/*
 * The MIT License
 *
 * Copyright (c) 2007 MIT 6.270 Robotics Competition
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <joyos.h>
#include <math.h>
#include <Point.h>

 #define LSB_US_PER_DEG 1386583
 #define GRYO 21
 #define MOTOR_L 2
 #define MOTOR_R 3
 #define DRIVE_TOLERANCE 150

uint8_t team_number[2] = {1,0};
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
	motor_brake(MOTOR_R);
	motor_brake(MOTOR_L);
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
	current_angle = gyro_get_degrees();
	float desired_angle = current_angle + degrees;
	if (robot_moving()) {

	}
	else {
		if (degrees>0) {
			while (gyro_get_degrees() < desired_angle) {
				set_velocities(-50, 50);
			}
		}
		else {
			while (gyro_get_degrees() > desired_angle) {
				set_velocities(50, -50);
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
	set_velocity(velocity);
	pause(millis);
	brake();
}

int usetup (void) {
	go_click();
	pause(500);
	gyro_init(GRYO, LSB_US_PER_DEG, 500);
	set_starting_loc(); 
    return 0;
}

int umain (void) {
    move_for_time(50, 2000);
    rotate(45);
    return 0;
}
