#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <moving.h>
#include <transport.h>
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
  gyro_set_degrees(get_vps_theta());

  // while(true) {
  //   aim_towards_target_vps((Point){0, -850});
  //   // aim_towards_target_vps(territories[0]);
  //   // aim_towards_target_vps(territories[1]);
  //   pause(10);
  // }

  int i_start = 0;
  float angle = atan2(get_vps_position().y, get_vps_position().x) * RADS_TO_DEGS;
  if (fabs(ang_diff(angle, 0)) > 90) {
    i_start = 3;
  }
  printf("i_start %i\n", i_start);

  // for (int i = i_start; i < i_start + 7; i++) {
  //   printf("[3] starting targeting %i\n", i);
  //   while(points_distance(get_vps_position(), territories[i]) > 250) {
  //     aim_towards_target_vps_gyro(territories[i % 6]);
  //     pause(6);
  //     print_vps_pos();
  //   }
  //   set_wheel_pows(0,0);
  //   printf("[3] reached target %i\n", i);
  //   pause(100);
  //   gyro_set_degrees(get_vps_theta());
  //   pause(100);
  // }

  // // move around the board twice
   for(int i=1;i<7;i++){
     move_to_territory_dr(i);
  //   set_wheel_pows(0, 0);
  //   pause(1000);
   }

  // //Move to 3 random territories
  // for(int i=0;i<3;i++){
  //  move_to_territory(get_time()%6);
  //  pause(500);
  // }

  return 0;
}
