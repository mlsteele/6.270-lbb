#ifndef B_H
#define B_H

#include "../moving.h"
void B() {
	set_velocity(0);
	while(1){
		Point goal = TARGET_POINT;
		Point current = CURRENT_POINT;

		float current_angle = CURRENT_ANGLE;
		float delta_theta = atan((goal.y-current.y)/(goal.x-current.x))*180/M_PI - current_angle;

		float dist = sqrt(pow((current.x-goal.x), 2)+pow(current.y-goal.y, 2));

		if(fabsf(delta_theta) > 1000/dist){
			set_velocity(0);
			rotate(delta_theta);
			set_velocity(150);
		}else{
			float vel_1 = 150 + delta_theta;
			float vel_2 = 150;

			if(delta_theta>0) {
				set_velocities(vel_1, vel_2);
			}
			else {
				set_velocities(vel_2, vel_1);
			}
		}

		pause(300);
	}
}
#endif