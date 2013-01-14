#include <moving.h>
#include "../cat_mouse/vps_data_daemon.h"
#include "fake_vps_daemon.h"

bool test = false;

void move_towards_target() {
	float dist_x;
	float dist_y;
	float dist;
	float desired_angle;
	float angle_threshold;
	printf("moving towards target\n");
	if (test) {
		printf("testing mode enabled\n");
	}
	while (1) {
		acquire(&vps_data_lock);
		if (test) {
			acquire(&fake_data_lock);
			dist_x = fake_active_target.x - vps_position.x;
			dist_y = fake_active_target.y - vps_position.y;
			printf("target = {%f,%f} ; current location = {%f,%f}", 
				fake_active_target.x, fake_active_target.y,
				vps_position.x, vps_position.y);
			release(&fake_data_lock);
		}
		else {
			dist_x = vps_active_target.x - vps_position.x;
			dist_y = vps_active_target.y - vps_position.y;
		}
		dist = sqrt(pow(dist_x, 2)+pow(dist_y, 2));
		desired_angle = atan(dist_y/dist_x);
		angle_threshold = 1000/dist; //this should be tweaked
		if(abs(vps_theta-desired_angle)>angle_threshold) {
			//wheels_brake();
			printf("desired angle = %f, current angle = %f, angle threshold = %f.\nbrake, rotate %f degrees\n", 
				desired_angle, vps_theta, angle_threshold, desired_angle-vps_theta);
			//rotate(desired_angle-vps_theta);
		}
		release(&vps_data_lock);
		//set_velocity(DEFAULT_SPEED);
		pause(1000);
	}
}