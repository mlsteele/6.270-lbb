
#define ENCODER_PIN 25

// Include headers from OS
#include <joyos.h>
#include "../encoder_utils.h"
#include <moving.h>
//#include "../cat_mouse/move_towards_target_smooth.h"

void move_towards_target_smooth(Point goal, Point current, float vps_theta) {
	//set_velocity(0);
	//while(1) {
		//acquire(&vps_data_lock);
		//Point goal = vps_active_target;
		//Point current = vps_position;

		float delta_theta = atan((goal.y-current.y)/(goal.x-current.x))*180/M_PI - vps_theta;
		//release(&vps_data_lock);

		float dist = sqrt(pow((current.x-goal.x), 2)+pow(current.y-goal.y, 2));

		printf("Goal: (%f,%f)\n", goal.x, goal.y);
		printf("Current: (%f,%f)\n", current.x, current.y);
		printf("Current theta: %f\n", vps_theta);
		printf("Delta theta: %f\n", delta_theta);
		printf("Dist: %f\n", dist);

		if(fabsf(delta_theta) > 1000/dist){
			printf("fabsf(%f) > %f\nStopping and rotating", delta_theta, 1000/dist);
			//set_velocity(0);
			//rotate(delta_theta);
			//set_velocity(150);
		}else{
			float vel_1 = 150 + delta_theta;
			float vel_2 = 150;

			if(delta_theta>0) {
				printf("set_velocities(%f,%f)\n",vel_1, vel_2);
			//	set_velocities(vel_1, vel_2);
			}
			else {
				printf("set_velocities(%f,%f)\n",vel_2, vel_1);
			//	set_velocities(vel_2, vel_1);
			}
		}

		//pause(300);
	//}
}

// usetup is called during the calibration period. 
int usetup (void) {
    return 0;
}

// Entry point to contestant code.
int umain (void) {
	Point[] targets = {{1000,1000},{0,500},{500,250}};
	Point[] currents = {{0,0},{0,0}{0,1000}};
	float[] thetas = {30., 10., 180.};
	for(int i=0; i>3; i++){
		move_towards_target_smooth(targets[i], currents[i], thetas[i]);
		pause(500);
	}
    return 0;
}
