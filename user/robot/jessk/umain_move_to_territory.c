#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <moving.h>
#include <math.h>
//#include "../cat_mouse/get_vps.h"
//#include "../cat_mouse/vps_utils.h"
#include <vps_data_daemon.h>
#include <territory.h>

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  territory_init();
  territory_check();
  vps_data_daemon_init();
  return 0;
}

int umain (void) {

	//move around the board twice
	for(int i=0;i<12;i++){
		move_to_next_territory();
	}

	pause(2000);

	// //Move to 3 random territories
	// for(int i=0;i<3;i++){
	// 	move_to_territory(get_time()%6);
	// 	pause(500);
	// }
	return 0;
}
