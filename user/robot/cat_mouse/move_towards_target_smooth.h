#ifndef _MOVE_TOWARDS_TARGET_SMOOTH_H_
#define _MOVE_TOWARDS_TARGET_SMOOTH_H_

#include <moving.h>

Point get_position() {
	return get_vps_position();
	//return get_hybrid_position();
}

float get_theta() {
	//return fmod(gyro_get_degrees(), 360);
	return get_vps_theta();
}

void rotate_to_angle(float theta_target, int direction) {
  float max_drive_speed = 0.35;
  float min_drive_speed = 0.12;
  float drive_speed = 0.32;
  float theta_thresh = 7;

  set_wheel_pows(-drive_speed * direction, drive_speed * direction);

  float angdiff = ang_diff(theta_target, get_vps_theta());
  
  printf("initial angdiff %f\n", angdiff);
  while(angdiff > theta_thresh) {
    // check in with vps
    if (get_us_since_vps_read() > 100000) {
      set_wheel_pows(0,0);
      printf("waiting for vps...");
      wait_for_vps_read();
      printf(" done\n");
    }

    angdiff = ang_diff(theta_target, get_vps_theta());
    printf("loop angdiff %f\n", angdiff);
    print_vps_pos();
    float drive_factor = fmin(angdiff / 90, 1);
    drive_speed = max_drive_speed * drive_factor + min_drive_speed;
    set_wheel_pows(-drive_speed * direction, drive_speed * direction);

    pause(5);
  }

  set_wheel_pows(0, 0);
}

void move_towards_target_smooth() {
	printf("Hello from move_towards_target_smooth()\n");
	set_wheel_pows(0,0);
	while(1) {
		printf("Hello from the while loop!\n" );
		//acquire(&vps_data_lock);
		//printf("Hello from acquire vps_data_lock\n");
		Point goal = vps_fake_active_target();
		Point current = get_position();
		float theta = get_theta();

		float delta_theta = -atan2((goal.y-current.y),(goal.x-current.x))*180/M_PI + theta;
		//release(&vps_data_lock);

		if(delta_theta < -180) {delta_theta += 360;}
		if(delta_theta > 180) {delta_theta -= 360;}
		float dist = sqrt(pow((current.x-goal.x), 2)+pow(current.y-goal.y, 2));

		printf("\nGoal: (%.1f,%.1f)\n", goal.x, goal.y);
		printf("Current: (%.1f,%.1f)\n", current.x, current.y);
		printf("Current theta: %.1f\n", theta);
		printf("Delta theta: %.1f\n", delta_theta);
		printf("Dist: %.3f\n", dist);

		if(fabsf(delta_theta) >= 90 || dist <= 200){
			printf("%.1f >= 90 || %.3f <= 200\nStopping and rotating\n", delta_theta, dist);

			set_wheel_pows(0,0);
			rotate(delta_theta);
			set_wheel_pows(150/255, 150/255);
		}else{
			float vel_1 = 150/255 + fabsf(delta_theta)/255;
			float vel_2 = 150/255;

			if(delta_theta>0) {
				printf("set_wheel_pows(%.1f,%.1f)\n",vel_1, vel_2);
				set_wheel_pows(vel_1, vel_2);
			}
			else {
				printf("set_wheel_pows(%.1f,%.1f)\n",vel_2, vel_1);
				set_wheel_pows(vel_2, vel_1);
			}
		}

		pause(300);
	}
}
#endif
