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
uint8_t num_balls = 0; //an estimate assuming everything we mined is in our basket

uint8_t order_of_preference[6];
uint8_t cw_board[6];
uint8_t ccw_board[6];

uint32_t elapsed_time() {
	return get_time()-start_time;
}

void explore() {
	//TODO
}

void claim_territory() {
	capture_attempts++;
	face_towards_gears();
	printf("done facing towards gears\n");
	terr = current_territory();
	Point pos = get_vps_position();
	while(points_distance(&pos,&gears[terr])>450){
		pos = get_vps_position();
		printf("current position:%f,%f\ncurrent territory:%d\ngears:%f,%f\ndistance:%f\n",
			get_vps_position().x,get_vps_position().y, current_territory(),
			gears[current_territory()].x, gears[current_territory()].y,
			points_distance(&pos,&gears[current_territory()])
		);
		aim_towards_target_vps_gyro(&gears[terr], FD_SPINSIDE);
		pause(10);
	}
	printf("close enough to gears\n");
	set_wheel_pows(0,0);
	uint32_t time = get_time();
	motor_set_vel(PIN_MOTOR_GEAR, 255 * get_team_color()==TEAM_RED ? -1 : 1);
	while (vps_owner(terr) != 8 && get_time()-time<5000) {}
	if (vps_owner(terr) != 8) {
		printf("timeout, didn't capture territory\n");
	}
	else {
		printf("done capturing territory\n");
	}
	motor_set_vel(PIN_MOTOR_GEAR, 0);
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

	while(points_distance(&pos,&mines[terr])>400){
		//move to mine
		pos = get_vps_position();
		printf("current position:%f,%f\ncurrent territory:%d\nmine:%f,%f\ndistance:%f",
			get_vps_position().x,get_vps_position().y, current_territory(),
			mines[current_territory()].x, mines[current_territory()].y,
			points_distance(&pos,&mines[current_territory()])
			);
		aim_towards_target_vps_gyro(&mines[terr], FD_SPINSIDE);
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
}

void dump_balls() {
	//TODO
	num_balls = 0;
}

void usetup() {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);
  vps_data_daemon_init();
  territory_init();
  order_of_preference[0] = terr;
  order_of_preference[1] = (terr + 1)%6;
  order_of_preference[2] = (terr + 5)%6;
  order_of_preference[3] = (terr + 2)%6;
  order_of_preference[4] = (terr + 4)%6;
  order_of_preference[5] = (terr + 3)%6;

  cw_board[0] = terr;
  cw_board[1] = (terr+5)%6;
  cw_board[2] = (terr+4)%6;
  cw_board[3] = (terr+3)%6;
  cw_board[4] = (terr+2)%6;
  cw_board[5] = (terr+1)%6;

  ccw_board[0] = terr;
  ccw_board[1] = (terr+1)%6;
  ccw_board[2] = (terr+2)%6;
  ccw_board[3] = (terr+3)%6;
  ccw_board[4] = (terr+4)%6;
  ccw_board[5] = (terr+5)%6;
}

void umain() {
	start_time = get_time();
	determine_team_color();
	explore();
	while (elapsed_time() < 120000) {
		if (elapsed_time() > 10000) {
			terr = current_territory();
			if (num_balls >= 20 || elapsed_time() > 110000) {
				dump_balls();
			}
			else if (vps_owner(terr)==8 && not_over_rate_limit(terr)) {
				printf("mining territory %d", terr);
				mine_territory();
				go_to_point(&territories[terr], false);
			}
			else if (vps_owner(terr) != 8 && capture_attempts<3){
				printf("claiming territory %d", terr);
				claim_territory();
				go_to_point(&territories[terr], false);
			}
			else {
				capture_attempts = 0;
				printf("moving to next territory %d", next_unowned_territory());
				uint8_t t= next_unowned_territory();
				if (t==-1) {
					//no unowned territories left
					uint8_t m = next_mineable_territory();
					if (m==-1) {
						//no territories under the rate limit
						if (num_balls > 0) {
							dump_balls();
						}
						else {
							//we have literally nothing better to do than wait
							pause(1000);
						}
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