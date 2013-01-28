#include <vps_data_daemon.h>
#include <territory.h>
#include <transport.h>
#include <Point.h>
#include <moving.h>
#include <hw_config.h>
#include <strategy.h>


void claim_territory() {
	face_towards_gears();
	uint8_t terr = current_territory();
	Point pos = get_vps_position();
	while(points_distance(&pos,&gears[terr])>355){
		pos = get_vps_position();
		printf("current position:%f,%f\ncurrent territory:%d\ngears:%f,%f\ndistance:%f",
			get_vps_position().x,get_vps_position().y, current_territory(),
			gears[current_territory()].x, gears[current_territory()].y,
			points_distance(&pos,&gears[current_territory()])
			);
		aim_towards_target_vps_gyro(&gears[terr], FD_SPINSIDE);
		pause(10);
	}
	set_wheel_pows(0,0);
	uint32_t time = get_time();
	motor_set_vel(PIN_MOTOR_GEAR, 255 * get_team_color()==TEAM_RED ? -1 : 1);
	while (vps_owner(terr) != 8 || get_time()-time<5000) {}

	//TODO set servos to turn gearbox until territory is captured.
	//should have a timeout to try readjusting postion after x seconds
}

void usetup() {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);
  vps_data_daemon_init();
  territory_init();
}

void umain() {

	determine_team_color();

	//while (get_time()-start_time < 120000) {
	//	move_to_territory(next_unowned_territory());
		claim_territory();
//	}
}