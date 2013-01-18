#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <moving.h>
#include <math.h>
#include "get_vps.h"

/*
			-------|-------
		   /   L   |   G   \
		  /G       |	   L\
		/	       |		  \
	   /\	   5   |	4	  /\
	 /	   \	   |	   /     \
	/L  	  \  //|\\ 	/		 G\
   /	        |  |  |			   \
   \	  0 	|ME| U|   	3	   /
    \G   	  /	 \\|//	\		 L/
     \	   /	   |	   \	 /
       \/	   1   |	2	  \/
       	\		   |		  /
       	  \L	   |	   G/
       	   \   G   |   L   /
       	    -------|-------

*/
uint32_t starting_time;

void explore() {
	uint8_t starting_territory = current_territory();
	move_to_territory(current_territory()+1);
	while(current_territory()!=starting_territory){
		move_to_territory(current_territory()+1);
	}
}
uint32_t time_left() {
	return 2*60*1000 - (get_time() - starting_time);
}
void move_to_base() {

	//Move to my side of the board:
	switch(current_territory()){
		case 0: break;
		case 1: break;
		case 5: break;
		case 2: 
			if(is_enemy_in_territory(1)) {
				move_to_territory(5);
			}else {move_to_territory(1);} break;
		case 4: 
			if(is_enemy_in_territory(5)) {
				move_to_territory(1);
			}else {move_to_territory(5);} break;
		case 3: 
			if(is_enemy_in_territory(4) || is_enemy_in_territory(5)){
				move_to_territory(1);
				else {
					move_to_territory(5);
				}
			} break;
	}
	//TODO figure out better coords than 0,0
	move_to({0,0});
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
	//TODO 
	return 0; //not for mock competition 2!
}
void move_to_territory(uint8_t territory){
	//It's someone else's job to make sure the enemy isn't in
	// the territory that is passed to this

	//IDGAF if I hit the enemy on the way there!! TODO fix that.

	//Is CCW the best way there?
	if((territory-current_territory())%6 <= 3) {
		for(uint8_t diff = territory-current_territory(); 
			diff > 0; 
			move_to_territory(current_territory()+1))
		{}
	}
	///START 
}
uint8_t next_unowned_territory() {

	return territory;
}
void claim_territory() {

}
bool can_gather_resources(uint8_t territory) {

	return can;
}
void gather_resources() {

}
int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  return 0;
}


int umain (void) {

	// THE HIGHEST LEVEL OF THOUGHT PROCESSES
	// NOW AVAILABLE IN GLORIOUS, GLORIOUS PSEUDOCODE

	starting_time = get_time();

	explore();

	while(1) {
		//Do I have to take a dump?
		if(numb_balls >= MAX_BALLS || time_left() > 20000) {
			//if you are full of balls or time is about to run out (20sec)
			move_to_base();
			drop_balls();
		} 

		//Should I move to another territory before I start claming shit?
		if (owner(current_territory())==me 
		 || is_enemy_in_territory(current_territory())){
			move_to_territory(next_unowned_territory());
		}
		claim_territory();

		//Can I gather resources from here?
		if(can_gather_resources(current_territory())){
			gather_resources();
		}
	}

    return 0;
}

