// Include headers from OS
#include <joyos.h>
#include <hw_config.h>
#include "../cat_mouse/vps_data_daemon.h"

int usetup (void) {
  return 0;
}

int umain (void) {
  printf("starting vps_data_daemon...");
  vps_data_daemon_init();
  printf(" done\n");

  while(1) {
    acquire(&vps_data_lock);
    Point goal = vps_active_target;
    Point current = vps_position;

    float theta = vps_theta;
    release(&vps_data_lock);
    
    printf("vps_active_target = \t%.2f, %.2f\n", goal.x, goal.y);
    printf("vps_position = \t%.2f, %.2f\n", current.x, current.y);
    printf("vps_theta = \t%.2f", theta);
    pause(120);
  }

  return 0;
}
