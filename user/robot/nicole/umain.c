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
#include <hw_config.h>
#include "../cat_mouse/move_towards_target_smooth.h"

 #define LSB_US_PER_DEG 1386583
 #define GRYO 21
 #define MOTOR_L 0
 #define MOTOR_R 1
 #define DRIVE_TOLERANCE 150

//uint8_t team_number[2] = {1,0};
Point current_loc;
float current_angle = 0; //probably want to set it to a different value

void set_starting_loc(void) {
	//use gps
	current_loc.x = 0;
	current_loc.y = 0;
}

void update_location() {
	//use gps
}

void deccelerate(float millis) {
	//deccelerates in the given amount of time
	//use jess's code
}


int usetup (void) {
	extern volatile uint8_t robot_id;
  	robot_id = 8;
	printf("Stabilizing\n");
	pause(500);
	printf("Initiating gyro\n");
	gyro_init(GRYO, LSB_US_PER_DEG, 500);
	printf("Gryoscope initated\n");
	set_starting_loc(); 
    return 0;
}

int umain (void) {
	vps_data_daemon_init();
	while (!get_vps_daemon_has_run()) {
		printf("Waiting for vps\n");
		pause(300);
	}
	//hybrid_position_daemon_init();

	move_towards_target_smooth();
	// printf("Setup complete\n");
	// vps_data_daemon_init();
	// move_towards_test_mode = true;
 //  printf("Initiated vps daemon\n");
	// if (move_towards_test_mode) {
	//  	fake_data_daemon_init();
	// 	printf("Initiated fake data daemon\n");
 //  }

 //  // wait for vps
 //  while(!get_vps_daemon_has_run()) {
 //    printf("waiting for vps to recv\n");
 //    pause(500);
 //  }

	// target_tracking();
	// //move_for_time(.5,1000);
	// // rotate(-90);
	// // pause(500);
	// // rotate(-90);
	// // pause(500);
	// // rotate(-90);
	// // pause(500);
	// // rotate(-90);
    return 0;
}
