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
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);
  return 0;
}

int umain (void) {
  vps_data_daemon_init();
  bind_gyro_to_vps();

  while(true) {
    vps_aim_towards_target((Point){0, -850});
    // vps_aim_towards_target(territories[0]);
    // vps_aim_towards_target(territories[1]);
    pause(10);
  }

  // // move around the board twice
  // for(int i=0;i<12;i++){
  //   move_to_next_territory();
  //   set_wheel_pows(0, 0);
  //   pause(1000);
  // }

  pause(2000);

  // //Move to 3 random territories
  // for(int i=0;i<3;i++){
  //  move_to_territory(get_time()%6);
  //  pause(500);
  // }
  return 0;
}
