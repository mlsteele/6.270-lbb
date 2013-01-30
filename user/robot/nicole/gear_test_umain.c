#include <vps_data_daemon.h>
#include <territory.h>
#include <transport.h>
#include <Point.h>
#include <moving.h>
#include <hw_config.h>
#include <strategy.h>

uint8_t terr = 0;
uint32_t start_time;
uint8_t capture_attempts = 0;
uint8_t failed_capture_attempts = 0;
uint8_t mining_attempts = 0;
uint8_t failed_mining_attempts = 0;
bool try_to_mine = true;
bool failure_mode = false;
uint8_t num_balls = 0; //an estimate assuming everything we mined is in our basket

uint8_t order_of_preference[6];
uint8_t cw_board[6];
uint8_t ccw_board[6];

uint32_t elapsed_time() {
	return get_time()-start_time;
}

void explore() {
	int i_init = current_territory();
    printf("i_init %i\n", i_init);
    for(int i = i_init; i <= i_init + 7; i++) {
	    int active_i = i % 6;
	    printf("active_i %i\n", active_i);
	    go_to_point(&territories[active_i], false);
  	}
}

void claim_territory() {
	capture_attempts++;
	go_to_point(&pre_gears[current_territory()], false);
    capture_gears(get_team_color());
}

uint8_t next_unowned_territory() {
	uint8_t t = -1;
	uint8_t enem_terr = territory_of_point(get_vps_antagonist());
	bool enem_flag = false;
	for (int i = 0; i<6 && t==-1; enem_flag ? i+=2 : i++) {
		if (vps_owner(order_of_preference[i]) != 8) {
			if (order_of_preference[i] == enem_terr)
			{
				enem_flag = true;
				//decrement i and then add 2 for each iteration of for loop to only check items of opposite parity of where enemy is
				//e.g. enemy is one territory left of us, so we'll only consider territories to the right.
				//bug or intended behavior? : won't consider going all the way around the board in the opposite direction, will just say there are no available territories
				i--;
			}
			else {
				t = order_of_preference[i];
			}
			
		}
	}
	return t;
}

uint8_t next_mineable_territory() {
	uint8_t t = -1;
	uint8_t enem_terr = territory_of_point(get_vps_antagonist());
	bool enem_flag = false;
	for (int i = 0; i<6 && t==-1; i++) {
		if (vps_owner(order_of_preference[i]) == 8 && not_over_rate_limit(order_of_preference[i])) {
			if (order_of_preference[i] == enem_terr)
			{
				enem_flag = true;
				//decrement i and then add 2 for each iteration of for loop to only check items of opposite parity of where enemy is
				//e.g. enemy is one territory left of us, so we'll only consider territories to the right.
				//bug or intended behavior? : won't consider going all the way around the board in the opposite direction, will just say there are no available territories
				i--;
			}
			else {
				t = order_of_preference[i];
			}
		}
	}
	return t;
}

void pull_lever() {
	servo_set_pos(PIN_SERVO_GEARS, SERVO_GEARS_UP);
	servo_set_pos(PIN_SERVO_GEARS, SERVO_GEARS_DN);
	servo_set_pos(PIN_SERVO_GEARS, SERVO_GEARS_UP);
}

void mine_territory() {
	face_towards_mine();
	terr = current_territory();
	Point pos = get_vps_position();
	uint32_t t = get_time();

	while(points_distance(&pos,&mines[terr])>1000 && get_time()-t < 10000){
		//move to mine
		pos = get_vps_position();
		printf("current position:%f,%f\ncurrent territory:%d\nmine:%f,%f\ndistance:%f",
			get_vps_position().x,get_vps_position().y, current_territory(),
			mines[current_territory()].x, mines[current_territory()].y,
			points_distance(&pos,&mines[current_territory()])
			);
		aim_towards_target_vps_gyro(&mines[terr], FD_LEVERSIDE);
		pause(10);
	}
	set_wheel_pows(0,0);

	uint16_t score = get_vps_score(); 
	pull_lever();
	uint8_t attempts = 0;
	while (get_vps_score()==score && attempts < 3) {
		
		//TODO reposition
		pull_lever();
		attempts++;
	}
	if (get_vps_score() != score)
	{
		for (int i = 0; i<4; i++) {
			pull_lever();
		}
		num_balls +=5;
	}
	else {
		mining_attempts++;
	}
}

void dump_balls() {
	//TODO
	num_balls = 0;
}

void usetup() {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  pause(700);
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 1000);
  vps_data_daemon_init();
  territory_init();
  gyro_set_degrees(get_vps_theta());
  order_of_preference[0] = terr;
  order_of_preference[1] = (terr + 1)%6;
  order_of_preference[2] = (terr + 5)%6;
  order_of_preference[3] = (terr + 2)%6;
  order_of_preference[4] = (terr + 4)%6;
  order_of_preference[5] = (terr + 3)%6;
}

void umain() {
	start_time = get_time();
	determine_team_color();
	explore();
	while (elapsed_time() < 120000) {
		if (elapsed_time() > 10000) {
			// if (failed_capture_attempts > 4) {
			// 	explore();
			// }
			if (failed_mining_attempts > 3) {
				try_to_mine = false;
			}
			terr = current_territory();
			if (num_balls >= 20) {
				dump_balls();
			}
			else if (vps_owner(terr)==8 && not_over_rate_limit(terr) && mining_attempts<3 && try_to_mine) {
				printf("mining territory %d", terr);
				mine_territory();
				go_to_point(&territories[terr], false);
			}
			else if (vps_owner(terr) != 8 && capture_attempts<3){
				claim_territory();
				go_to_point(&territories[terr], false);
			}
			else {
				if (capture_attempts==3) {
					failed_capture_attempts++;
				}
				if (mining_attempts == 3) {
					failed_mining_attempts++;
				}
				capture_attempts = 0;
				mining_attempts = 0;
				printf("moving to next territory %d", next_unowned_territory());
				uint8_t t= next_unowned_territory();
				if (t==-1) {
					//no unowned territories left
					uint8_t m = next_mineable_territory();
					if (m==-1) {
						// //no territories under the rate limit
						// if (num_balls > 0) {
						// 	//dump_balls();
						// }
						// else {
						// 	//we have literally nothing better to do than wait
						// 	pause(1000);
						// }
						pause(1000);
					}
					else {
						go_to_territory(m, true);
					}	
				}
				else {
					go_to_territory(t, true);
				}
				
			}
		}
		
	}
}