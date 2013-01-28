#include <territory.h>
#include <moving.h>
#include <hwconfig.h>
#include <vps_data_daemon.h>
#include "../jessk/encoder_daemon.h"
#include <transport.h>
#define me 8

uint32_t starting_time;
Point base_5;
Point base_0;
Point base_1;

uint32_t time_left() {
	return 2*60*1000 - (get_time() - starting_time);
}

uint8_t owner(uint8_t territory) {
	return vps_owner(territory);
}

bool is_enemy_in_territory(uint8_t territory){
	bool ret = enemy_location()==current_territory();
	//return ret;
	return 0; //TODO change this when not in mock competition
}

uint8_t next_unowned_territory() {
	uint8_t cur = current_territory();
	uint8_t order_of_preference[5] = {
		(cur + 1)%6,
		(cur + 5)%6,  //cur-1
		(cur + 2)%6, 
		(cur + 4)%6,  //cur-2 
		(cur + 3)%6
	};
	for(uint8_t i=0; i<5; i++){
		if(owner(order_of_preference[i])!=me
		&& is_enemy_in_territory(order_of_preference[i])) {
			//TODO is_enemy_in_territory is going to act weird.
			//might want to avoid cur+2 if enemy is in cur+1.
			return order_of_preference[i];
		}
	}
	return order_of_preference[0];
}
void move_to_target(Point p, int dir) {
	//generic moving to a point using aim_towards_target_vps
	while (points_distance(get_vps_position(), p) > 200) {
		aim_towards_target_vps(p, dir);
		pause(10);
	}
	set_wheel_pows(0,0);
}
void move_to_base() {
	//Move to territory 5,0,or 1 then dump there:
	switch(current_territory()){
		case 0: move_to_target(base_0,1); break;
		case 1: move_to_target(base_1,1); break;
		case 5: move_to_target(base_5,1); break;
		case 2: 
			if(is_enemy_in_territory(1)) {
				move_to_territory(5);
				move_to_target(base_5,1);
			}else {
				move_to_territory(1);
				move_to_target(base_1,1);
			} break;
		case 4: 
			if(is_enemy_in_territory(5)) {
				move_to_territory(1);
				move_to_target(base_1,1);
			}else {
				move_to_territory(5);
				move_to_target(base_5,1);
			} break;
		case 3: 
			if(is_enemy_in_territory(4) || is_enemy_in_territory(5)){
				move_to_territory(1);
				move_to_target(base_1,1); 
			}
			else {
				move_to_territory(5);
				move_to_target(base_5,1);
			} break;		 
	}
}
void claim_territory() {
	while(points_distance(get_vps_position(),gears[current_territory()])>100 && !is_robot_stuck()){
		aim_towards_target_vps(gears[current_territory()], -1);
		pause(10);
	}
	set_wheel_pows(0,0);

	//TODO set servos to turn gearbox until territory is captured.
	//should have a timeout to try readjusting postion after x seconds
}

bool can_gather_resources(uint8_t territory) {
	bool can = has_balls_remaining(territory) && not_over_rate_limit(territory);
	return can;
}
void gather_resources() {
//TODO
}

// void drop_balls() {
// 	int starting_score = vps_score();
// 	open_gate();
// 	while(starting_score + numb_balls*40 < vps_score()) {
// 		pause(100);
// 	}
// 	close_gate();
// }

void explore() {
	// borked
	// uint8_t starting_territory = current_territory();
	// move_to_territory((current_territory()+1)%6);
	// while(current_territory()!=starting_territory){
	// 	move_to_territory((current_territory()+1)%6);
	// }

	//from umain_move_to_territory.c
	 // move around the board twice
  for(int i=0;i<12;i++){
  	printf("Going to explore next territory");
    move_to_next_territory();
    set_wheel_pows(0, 0);
    printf("In next territory");
    pause(1000);
  }
  pause(2000);
  return 0;
}

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);
  vps_data_daemon_init();
  encoder_daemon_init();
  territory_init();
  return 0;
}

int umain (void) {
	gyro_set_degrees(get_vps_theta());
	starting_time = get_time();
	
	base_5 = (Point){-288, -167};
	base_0 = (Point){0,    -333};
	base_1 = (Point){288,  -167};

//	explore();

	while(1) {
		if(
			//numb_balls >= MAX_BALLS ||
			 time_left() < 20000) {
			printf("Competition almost over, moving to base");
			//if you are full of balls or time is about to run out (20sec)
			move_to_base();
		//	drop_balls(); TODO
		} 

		//Should I move to another territory before I start claming shit?
		if (owner(current_territory())==me 
		// || is_enemy_in_territory(current_territory())
			){
			printf("I own current territory (%d), moving to territory %d", current_territory(), next_unowned_territory());
			move_to_territory(next_unowned_territory());
		}
		printf("attempting to claim territory %d", current_territory());
		claim_territory();

		//Can I gather resources from here?
		if(can_gather_resources(current_territory())){
			printf("can gather resources in territory %d", current_territory());
			gather_resources();
		}
	}

    return 0;
}