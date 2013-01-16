#ifndef _MOVE_TOWARDS_TARGET_SMOOTH_H_
#define _MOVE_TOWARDS_TARGET_SMOOTH_H_

#include <moving.h>
#include <hybrid_integrator_daemon.h>
#include "../nicole/fake_vps_daemon.h"

Point get_position() {
	return get_vps_position();
	//return get_hybrid_position();
}

float get_theta() {
	return gyro_get_degrees();
	//return get_vps_theta();
}

Point get_target() {
	//Point tar = {100,100};
	//return get_fake_vps_active_target();
	return get_vps_active_target();
	//return tar;
}

float ang_diff(float a, float b) {
  return fmin(a - b, a - b + 360);
}

void rotate_to_angle(float theta_target) {
  float max_drive_speed = 0.35;
  float min_drive_speed = 0.12;
  float drive_speed = 0.32;
  float theta_thresh = 7;
  float angdiff = ang_diff(theta_target, get_theta());
  int direction = ang_diff>0 ? 1 : -1;
  set_wheel_pows(-drive_speed * direction, drive_speed * direction);
  
  printf("initial angdiff %f\n", angdiff);
  while(angdiff > theta_thresh) {
    // check in with vps
    // if (get_us_since_vps_read() > 100000) {
    //   set_wheel_pows(0,0);
    //   printf("waiting for vps...");
    //   wait_for_vps_read();
    //   printf(" done\n");
    // }

    angdiff = ang_diff(theta_target, get_theta());
    printf("loop angdiff %f\n", angdiff);
  //  print_vps_pos();
    float drive_factor = fmin(angdiff / 90, 1);
    drive_speed = max_drive_speed * drive_factor + min_drive_speed;
    printf("setting wheel pows {%f, %f)", -drive_speed*direction, drive_speed*direction);
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
		Point goal = get_target();
		Point current = get_position();
		float theta = get_theta();
		float desired_theta = atan2((goal.y-current.y),(goal.x-current.x))*180/M_PI;
		float delta_theta = desired_theta - theta;
		//release(&vps_data_lock);

		if(delta_theta < -180) {delta_theta += 360;}
		if(delta_theta > 180) {delta_theta -= 360;}
		float dist = sqrt(pow((current.x-goal.x), 2)+pow(current.y-goal.y, 2));

		printf("\nGoal: (%.1f,%.1f)\n", goal.x, goal.y);
		printf("Current: (%.1f,%.1f)\n", current.x, current.y);
		printf("Current theta: %.1f\n", theta);
		printf("Delta theta: %.1f\n", fmod(delta_theta,360));
		printf("Dist: %.3f\n", dist);

		if(fmod(delta_theta,360) >= 45){
			printf("%.1f >= 45\n", delta_theta);

			set_wheel_pows(0,0);
			rotate_to_angle(desired_theta);
			printf("Done rotating, now moving\n");
			set_wheel_pows(.6, .6);
		}else{
			float vel_1 = .6 + fabsf(fmod(delta_theta, 360)/100.f);
			float vel_2 = .6;

			if(delta_theta>0) {
				printf("set_wheel_pows(%.1f,%.1f)\n",vel_1, vel_2);
				set_wheel_pows(vel_1, vel_2);
			}
			else {
				printf("set_wheel_pows(%.1f,%.1f)\n",vel_2, vel_1);
				set_wheel_pows(vel_2, vel_1);
			}
		}

		pause(500);
	}
}
#endif
