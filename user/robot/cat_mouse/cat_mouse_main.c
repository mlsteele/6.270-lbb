#include <joyos.h>
#include <hw_config.h>
#include "./vps_data_daemon.h"
#include "./encoder_position_daemon.h"
#include "./move_towards_target_smooth.h"

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;

  printf("starting daemons...");
  vps_data_daemon_init();
  encoder_position_daemon_init();
  printf(" done\n");

  return 0;
}

int umain (void) {
  printf("entering hunting loop\n");
  while(1) {
    printf("calling move_towards_target_smooth...\n");
    move_towards_target_smooth();
    printf(" done\n");
    pause(10);
  }

  return 0;
}
