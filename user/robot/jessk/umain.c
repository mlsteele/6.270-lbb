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
uint32_t starting_time;;

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

}
void drop_balls() {

}
uint8_t owner(uint8_t territory) {

	return owner;
}
bool is_enemy_in_territory(uint8_t territory){

	return is;
}
void move_to_territory(uint8_t territory){

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

