#include ../moving.h

void move_towards_target() {
	float dist_x;
	float dist_y;
	float dist;
	float desired_angle;
	float angle_threshold;
	while (1) {
		dist_x = active_target.x - current_loc.x;
		dist_y = active_target.y - current_loc.y;
		dist = sqrt(pow(dist_x, 2)+pow(dist_y, 2));
		desired_angle = atan(dist_y/dist_x);
		current_angle = get_heading();
		angle_threshold = 1000/dist; //this should be tweaked
		if(abs(current_angle-desired_angle)>angle_threshold) {
			brake();
			rotate(desired_angle-current_angle);
		}
		set_velocity(SPEED);
		pause(300);
	}
}