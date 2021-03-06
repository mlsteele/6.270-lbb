#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <moving.h>
#include <math.h>
#include <territory.h>
#include "get_vps.h"
#include "encoder_daemon.h"

#define me 8
/*
            -------|-------
           /   L   |   G   \
          /G       |       L\
        /          |          \
       /\      5   |    4     /\
     /     \       |       /     \
    /L        \  //|\\  /        G\
   /            |  |  |            \
   \      0     |ME| U|     3      /
    \G        /  \\|//  \        L/
     \     /       |       \     /
       \/      1   |    2     \/
        \          |          /
          \L       |       G/
           \   G   |   L   /
            -------|-------

*/
uint32_t starting_time;
Point base_5;
Point base_0;
Point base_1;

void explore() {
	uint8_t starting_territory = current_territory();
	move_to_territory((current_territory()+1)%6);
	while(current_territory()!=starting_territory){
		move_to_territory((current_territory()+1)%6;
	}
}
uint32_t time_left() {
	return 2*60*1000 - (get_time() - starting_time);
}
void move_to_base() {

	//Move to territory 5,0,or 1 then dump there:
	switch(current_territory()){
		case 0: move_to(base_0); break;
		case 1: move_to(base_1); break;
		case 5: move_to(base_5); break;
		case 2: 
			if(is_enemy_in_territory(1)) {
				move_to_territory(5);
				move_to(base_5);
			}else {
				move_to_territory(1);
				move_to(base_1);
			} break;
		case 4: 
			if(is_enemy_in_territory(5)) {
				move_to_territory(1);
				move_to(base_1);
			}else {
				move_to_territory(5);
				move_to(base_5);
			} break;
		case 3: 
			if(is_enemy_in_territory(4) || is_enemy_in_territory(5)){
				move_to_territory(1);
				move_to(base_1);
				else {
					move_to_territory(5);
					move_to(base_5);
				}
			} break;
	}
}
void drop_balls() {
	int starting_score = vps_score();
	open_gate();
	while(starting_score + numb_balls*40 < vps_score()) {
		pause(100);
	}
	close_gate();
}
uint8_t owner(uint8_t territory) {
	return vps_owner(territory);
}
bool is_enemy_in_territory(uint8_t territory){
	bool ret = enemy_location()==current_territory();
	//return ret;
	return 0; //TODO change this when not in mock competition
}
void move_to_territory(uint8_t territory){
	//It's someone else's job to make sure the enemy isn't in
	// the territory that is passed to this

	//IDGAF if I hit the enemy on the way there!! TODO fix that.

	//Is CCW the best way there?
	int dir;
	if((territory-current_territory())%6 <= 3) {
		dir = 1;
	} else {dir = -1;}
	for(uint8_t diff = territory-current_territory(); 
		diff != 0; 
		move_to(territories[(current_territory()+dir)%6]);
	{}
	 
}
uint8_t next_unowned_territory() {
	uint8_t cur = current_territory();
	uint8_t order_of_preference[5] = {
		(cur + 1)%6,
		(cur + 5)%6,
		(cur + 2)%6, 
		(cur + 4)%6, 
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
}
void claim_territory() {
	while(find_distance(territories[i])>100 && !robot_stuck()){
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
int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  vps_data_daemon_init();
  encoder_daemon_init();
  return 0;
}


int umain (void) {

	// THE HIGHEST LEVEL OF THOUGHT PROCESSES
	// NOW AVAILABLE IN GLORIOUS, GLORIOUS PSEUDOCODE

	starting_time = get_time();

	//TODO transform function

	
	base_5 = {-288, -167};
	base_0 = {0,    -333};
	base_1 = {288,  -167};

	explore();

	while(1) {
		//Do I have to take a dump?
		if(numb_balls >= MAX_BALLS || time_left() < 20000) {
			//if you are full of balls or time is about to run out (20sec)
			move_to_base();
			drop_balls();
		} 

		//Should I move to another territory before I start claming shit?
		if (owner(current_territory())==me 
		 || is_enemy_in_territory(current_territory())){
			printf("moving to territory %d", next_unowned_territory());
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

