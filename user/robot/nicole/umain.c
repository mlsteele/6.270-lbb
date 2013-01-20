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
#include "../cat_mouse/get_vps.h"
#include "../cat_mouse/vps_utils.h"

 #define LSB_US_PER_DEG 1386583
 #define GRYO 21
 #define MOTOR_L 0
 #define MOTOR_R 1
 #define DRIVE_TOLERANCE 150
 #define CENTER_BOARD_RADIUS 300  
uint8_t start_time;
uint8_t current_territory;

int usetup (void) {
	extern volatile uint8_t robot_id;
  	robot_id = 8;
  	start_time = get_time();
    return 0;
}

int umain (void) {
	// while(1) {
	// 	current_territory = current_territory();
	// 	while (get_time() < start_time+10000) {
	// 		move_to_territory(current_territory + 1);
	// 	}
	// }
	
    return 0;
}

void move_to(Point p) {
	if (can_take_straight_path(get_vps_current(), p)                                                                                                           can_take_straight_path(get_vps_current(),p)) {
		vps_rotate(target_theta(p));
		vps_drive_towards(p);
	}
	else {                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     9
		//move in circular path
	}
}

bool can_take_straight_path(Point a, Point b) {
	bool can = true;
	float slope = (b.y-a.y)/(b.x-a.x);
	float y_val = a.y;
	for (float x_val = a.x; x_val<b.x && can; x_val++) {
		y_val += slope;
		if (pow(x_val,2) + pow(y_val,2) <= 225) {
			//straight path goes through center hole
			can = false;
		}
	}
	return can;
}
                                                                                                                                                                                                                                                                                       