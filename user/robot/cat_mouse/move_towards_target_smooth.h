#ifndef _B_H_
#define _B_H_

#include <moving.h>

void move_towards_target_smooth() {
	set_velocity(0);
	while(1) {
		acquire(&vps_data_lock);
		Point goal = vps_active_target;
		Point current = vps_position;

		float delta_theta = -atan((goal.y-current.y)/(goal.x-current.x))*180/M_PI + vps_theta;
		release(&vps_data_lock);

		if(delta_theta < -180) {delta_theta += 360;}
		if(delta_theta > 180) {delta_theta -= 360;}
		float dist = sqrt(pow((current.x-goal.x), 2)+pow(current.y-goal.y, 2));

		if(fabsf(delta_theta) >= 90 || dist <= 200){
			set_velocity(0);
			rotate(delta_theta);
			set_velocity(150);
		}else{
			float vel_1 = 150/255 + fabsf(delta_theta)/255;
			float vel_2 = 150/255;

			if(delta_theta>0) {
				set_wheel_pows(vel_1, vel_2);
			}
			else {
				set_wheel_pows(vel_2, vel_1);
			}
		}

		pause(300);
	}
}
#endif
