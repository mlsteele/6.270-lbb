#include <joyos.h>
#include "./vps_data_daemon.h"

int usetup (void) {
  return 0;
}


int umain (void) {
  printf("starting vps_data_daemon...");
  vps_data_daemon_init();
  printf(" done\n");

  printf("entering hunting loop\n", );
  while(1) {
    printf("calling move_towards_target...");
    move_towards_target();
    printf(" done\n");
    pause(10);
  }

  return 0;
}
