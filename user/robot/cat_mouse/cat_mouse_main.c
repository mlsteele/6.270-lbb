#include <joyos.h>
#include <hw_config.h>
#include "./vps_data_daemon.h"
#include "./encoder_position_daemon.h"
#include "./move_towards_target_smooth.h"

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;

  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);

  printf("starting daemons...");
  vps_data_daemon_init();
  //encoder_position_daemon_init();
  printf(" done\n");

  return 0;
}

int umain (void) {
  while(!get_vps_daemon_has_run()) {
    printf("waiting for vps to recv\n");
    pause(500);
  }
  printf("entering hunting loop\n");
  while(1) {
    printf("calling move_towards_target_smooth...\n");
    move_towards_target_smooth();
    printf(" done\n");
    pause(10);
  }

  return 0;
}
