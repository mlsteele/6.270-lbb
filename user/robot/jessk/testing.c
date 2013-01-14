#include <stdio.h>
#include <math.h>
typedef struct {
	float x;
	float y;
} Point;

void move_towards_target_smooth(Point goal, Point current, float vps_theta) {
	//set_velocity(0);
	//while(1) {
		//acquire(&vps_data_lock);
		//Point goal = vps_active_target;
		//Point current = vps_position;

		float delta_theta = -atan((goal.y-current.y)/(goal.x-current.x))*180/M_PI + vps_theta;
		//release(&vps_data_lock);

		float dist = sqrt(pow((current.x-goal.x), 2)+pow(current.y-goal.y, 2));
		if(delta_theta < -180) {delta_theta += 360;}
		if(delta_theta > 180) {delta_theta -= 360;}
		printf("\nGoal: (%.1f,%.1f)\n", goal.x, goal.y);
		printf("Current: (%.1f,%.1f)\n", current.x, current.y);
		printf("Current theta: %.1f\n", vps_theta);
		printf("Delta theta: %.1f\n", delta_theta);
		printf("Dist: %.3f\n", dist);

		if(fabsf(delta_theta) >= 90 || dist <= 200){
			printf("%.1f >= 90 || %.3f <= 200\nStopping and rotating\n", delta_theta, dist);
			//set_velocity(0);
			//rotate(delta_theta);
			//set_velocity(150);
		}else{
			//printf("fabsf(%.3f) < %.3f\n", delta_theta, dist/10);
			float vel_1 = 150/255 + fabsf(delta_theta)/255;
			float vel_2 = 150/255;

			if(delta_theta>0) {
				printf("set_velocities(%.1f,%.1f)\n",vel_1, vel_2);
			//	set_velocities(vel_1, vel_2);
			}
			else {
				printf("set_velocities(%.1f,%.1f)\n",vel_2, vel_1);
			//	set_velocities(vel_2, vel_1);
			}
		}

		//pause(300);
	//}
}

// Entry point to contestant code.
int main () {
	//Point targets[] = {{1000,1000},{0,500},{500,250}};
	printf("HELLO WORLD\n\n");
	Point targets[3]; 
	targets[0].x = 1000; 	targets[0].y = 1000;
	targets[1].x = 0; 		targets[1].y = 500;
	targets[2].x = 500;		targets[2].y = 250;
	Point currents[3]; 
	currents[0].x = currents[0].y = 0;
	currents[1].x = currents[1].y = 0;
	currents[2].x = 0; currents[2].y = 1000;
	float thetas[] = {30., 10., 180.};
	int i;
	for(i=0; i<3; i++){
		move_towards_target_smooth(targets[i], currents[i], thetas[i]);
		//pause(500);
	}
	Point f={1000,1000};  Point g = {0,0};
	move_towards_target_smooth(f,g, 60);
    return 0;
}
