#include <vps_data_daemon.h>
#include <territory.h>
#include <transport.h>
#include <Point.h>
#include <moving.h>
#include <hw_config.h>

void claim_territory() {
	while(points_distance(get_vps_position(),gears[current_territory()])>100){
		printf("current position:%f,%f\ncurrent territory:%d\ngears:%f,%f\ndistance:%f",
			get_vps_position().x,get_vps_position().y, current_territory(),
			gears[current_territory()].x, gears[current_territory()].y,
			points_distance(get_vps_position(),gears[current_territory()])
			);
		vps_aim_towards_target(gears[current_territory()], -1);
		pause(10);
	}
	set_wheel_pows(0,0);

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
	//move_to_territory(next_unowned_territory());
	claim_territory();
}