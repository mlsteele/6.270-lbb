float find_distance(Point target) {
	Point current = get_vps_current();
	return sqrt(pow(target.y-current.y,2)+pow(target.x-current.x,2));
}

float target_theta(Point target) {
	Point current = get_vps_current();
	return atan2(target.y-current.y,target.x-current.x) * 180./M_PI;
}

void vps_rotate(float target_theta) {
	printf("Rotating from %.2f to %.2f\n", get_vps_theta(), target_theta);
	while(fabs(ang_diff(target_theta, get_vps_theta())) > 5) {
		printf("\t%.2f\n", get_vps_theta());
		int direction = ang_diff(target_theta, get_vps_theta()) > 0 ? 1 : -1;
		set_wheel_pows(-direction * 0.3, direction * 0.3);
	}
	set_wheel_pows(0,0);
}

void vps_drive_towards(Point target) {
	printf("Driving towards (%.2f, %.2f)\n", target.x, target.y);
	while(fabs(target_theta(target) - get_vps_theta()) < 50) {
		printf("\t%.2fmm away\n", find_distance(target));
		set_wheel_pows(0.5,0.5);
	}
	set_wheel_pows(0,0);
}