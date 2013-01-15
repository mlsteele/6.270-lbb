#include <hw_config.h>
#include <hybrid_integrator_daemon.h>
#include <moving.h>
#include <vps_data_daemon.h>
#include "./fake_vps_daemon.h"

bool move_towards_test_mode = false;

Point get_position() {
	return get_vps_position();
}

float get_theta() {
	return get_vps_theta();
}

void target_tracking() {
	float dist_x;
	float dist_y;
	float dist;
	float desired_angle;
	float angle_threshold;
	printf("moving towards target\n");
	if (move_towards_test_mode) {
		printf("testing mode enabled\n");
	}
	printf("OMGWTF\n");
	while (1) {
		printf("target tracking loop cycle\n");
		if (move_towards_test_mode) {
			acquire(&fake_data_lock);
			dist_x = fake_active_target.x - get_position().x;
			dist_y = fake_active_target.y - get_position().y;
			printf("target = {%f,%f} ; current location = {%f,%f}\n", 
				fake_active_target.x, fake_active_target.y,
				get_position().x, get_position().y);
			release(&fake_data_lock);
		}
		else {
			dist_x = get_vps_active_target().x - get_position().x;
			dist_y = get_vps_active_target().y - get_position().y;
		}
		dist = sqrt(pow(dist_x, 2)+pow(dist_y, 2));
		desired_angle = atan2(dist_y,dist_x)*180/M_PI;
		// angle_threshold = 1000/dist; //this should be tweaked
		angle_threshold = 20; //this should be tweaked
		printf("desired angle = %f, current angle = %f, angle threshold = %f.\n",
			desired_angle, get_theta(), angle_threshold);
		if(abs(get_theta()-desired_angle)>angle_threshold) {
			wheels_brake();
			printf("brake, rotate %f degrees\n", desired_angle-get_theta());
			rotate(desired_angle-get_theta());
		}
		set_wheel_pows(DEFAULT_SPEED, DEFAULT_SPEED);
		pause(1000);
	}
}
