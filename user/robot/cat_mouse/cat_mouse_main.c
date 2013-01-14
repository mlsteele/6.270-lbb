#include <joyos.h>
#include <hw_config.h>
#include "./vps_data_daemon.h"
#include "./move_towards_target_smooth.h"

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  return 0;
}

int umain (void) {
  printf("starting vps_data_daemon...");
  vps_data_daemon_init();
  printf(" done\n");

  printf("entering hunting loop\n");
  //while(1) {
    printf("calling move_towards_target_smooth...\n");
    move_towards_target_smooth();
    printf(" done\n");
  //  pause(10);
  //}

  return 0;
}
