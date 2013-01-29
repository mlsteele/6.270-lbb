#include <hw_config.h>
#include <territory.h>
#include <vps_data_daemon.h>
#include <Point.h>
#include <util.h>
#include <math.h>
#include <moving.h>

uint32_t start_time;

void recalibrate_gyro(){
	//only use when the robot is standing still
	gyro_set_degrees(get_vps_theta());
}

void rotate_to(float deg) {
	int dir = ang_diff(gyro_get_degrees(), deg) > 0 ? 1 : -1;
	if (dir>0) {
		set_wheel_pows(.7, -.7);
	}
	else {
		set_wheel_pows(-.7, .7);
	}
	while (fabs(ang_diff(gyro_get_degrees(),deg)) > 5) {
		pause(10);
	} 
	set_wheel_pows(0,0);
}

void rotate(float deg) {
	rotate_to(get_vps_theta() + deg);
}


void drive_to_point(Point* p, uint8_t front) {
	//rotates if rotate is true, and then moves in a straight line to a point
	//1 for lever side, -1 for gear side, 0 for no preference
	Point current = get_vps_position();
	float desired_angle = atan2((*p).y-current.y,(*p).x-current.x) * 180./M_PI;
	if (front==0 && fabs(ang_diff(gyro_get_degrees(),desired_angle))>90) {
		rotate_to(-fmod(desired_angle,90));
		set_wheel_pows(-.7,-.7);
	}
	else {
		rotate_to(front>0 ? desired_angle : desired_angle + 180);
		if (front>0) {
			set_wheel_pows(.7,.7);
		}
		else {
			set_wheel_pows(-.7,-.7);
		}
	}
	while (points_distance(p, &current) > 300) {
		pause(10);
	}
	set_wheel_pows(0,0);
}

void drive_to_territory(uint8_t t) {
	drive_to_point(&territories[t], 0);
}

void drive_to_gearbox() {
	drive_to_point(&gears[current_territory()], -1);
}

void drive_to_mine() {
	drive_to_point(&mines[current_territory()], 1);
}

void pull_lever() {
	servo_set_pos(PIN_SERVO_GEARS, SERVO_GEARS_UP);
	servo_set_pos(PIN_SERVO_GEARS, SERVO_GEARS_DN);
	servo_set_pos(PIN_SERVO_GEARS, SERVO_GEARS_UP);
}

uint32_t elapsed_time() {
	return get_time()-start_time;
}

void explore() {
	uint8_t start = current_territory();
	uint8_t terr = (start + 1) % 6;
	while (terr != start) {
		printf("elapsed time:%d",elapsed_time());
		drive_to_point(&territories[terr], true);
		terr = (terr+1)%6;
	}
	printf("done exploring\n");
}

void usetup() {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);
  vps_data_daemon_init();
  territory_init();
}

void umain() {
	start_time = get_time();
	// printf("turning gears CW\n");
	// motor_set_vel(PIN_MOTOR_GEAR, -255);
	// pause(1000);
	// printf("stopping gears\n");
	// motor_set_vel(PIN_MOTOR_GEAR, 0);
	// pause(1000);

	// printf("turning gears CCW\n");
	// motor_set_vel(PIN_MOTOR_GEAR, 255);
	// pause(1000);
	// printf("stopping gears\n");
	// motor_set_vel(PIN_MOTOR_GEAR, 0);
	// pause(1000);
	// printf("driveing to next territory using drive_to_point\n");
	// drive_to_point(&territories[(current_territory()+5)%6], true);
	// pause(1000);

	// printf("driveing to next territory using aim_towards_target\n");
	// Point p = territories[((current_territory()+5)%6)];
	// Point pos = get_vps_position();
	// while (points_distance(&p,&pos) > 200) {
	// 	pos = get_vps_position();
	// 	aim_towards_target_vps_gyro(&p, FD_LEVERSIDE);
	// }
	// pause(1000);

	// printf("driveing to next territory using drive_to_territory");
	// drive_to_territory((current_territory()+5)%6, true);
	// pause(1000);

	// printf("facing towards gears");
	// face_towards_gears();

	// pause(1000);

	// printf("facing towards mine");
	// face_towards_mine();

	// printf("end");
	drive_to_territory(current_territory());
	explore();
	//face_towards_gears();
	drive_to_gearbox();
	motor_set_vel(PIN_MOTOR_GEAR, 255);
	uint32_t t = get_time();
	recalibrate_gyro();
	while(vps_owner(current_territory())!=8 && get_time()-t < 5000){}
	motor_set_vel(PIN_MOTOR_GEAR, 0);
	printf("Done capturing or timeout\n");
	drive_to_point(&territories[current_territory()], 1);
	//face_towards_mine();
	drive_to_mine();
	recalibrate_gyro();
	for (int i = 0; i<5; i++) {
		pull_lever();
	}
	printf("done testing");
}