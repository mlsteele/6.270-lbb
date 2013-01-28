// Include headers from OS
#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <territory.h>
#include <moving.h>
#include <vps_data_daemon.h>
#include <strategy.h>
#include <transport.h>

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;

  printf("stabilizing... ");
  pause(700);
  printf("done\n");
  territory_init();
  printf("calibrating gyro... ");
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 1000);
  printf("done\n");
  vps_data_daemon_init();
  pause(300);
  printf("binding gyro to vps coordinates... ");
  gyro_set_degrees(get_vps_theta());
  printf("done\n");
  determine_team_color();

  return 0;
}

int umain (void) {
  int i_init = current_territory();
  printf("i_init %i\n", i_init);
  for(int i = i_init; i < i_init + 7; i++) {
    printf("target i %i\n", i);
    go_to_territory(i, false);
  }

  // printf("[1] starting territory is %i\n", current_territory());
  // go_to_territory(current_territory() + 5);
  // pause(300);
  // go_to_territory(current_territory() + 1);
  // pause(300);
  capture_gears();
  go_to_territory(current_territory(), false);
  pause(500);
  mine_resources();

  set_wheel_pows(0,0);
  printf("umain done\n");
  return 0;
}
