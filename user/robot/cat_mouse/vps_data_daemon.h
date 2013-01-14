// Periodically updates and converts values from the vps system.
// When using vps data be sure to handle the vps_data_lock

#ifndef _VPS_DATA_DAEMON_H_
#define _VPS_DATA_DAEMON_H_

#include <joyos.h>
#include <Point.h>

#define UNITS_VPS_TO_MM 0.5953
#define UNITS_VPS_TO_DEG 0.08789;

Point vps_active_target;
Point vps_position;
float vps_theta;

bool vps_daemon_has_run = false;
struct lock vps_data_lock;

void convert_vps_pos_to_mm_pos(board_coord* src, Point* target) {
  target->x = src->x * UNITS_VPS_TO_MM;
  target->y = src->y * UNITS_VPS_TO_MM;
}

void vps_download_info() {
  acquire(&vps_data_lock);

  copy_objects();

  convert_vps_pos_to_mm_pos(&game.coords[0], &vps_position);
  convert_vps_pos_to_mm_pos(&game.coords[1], &vps_active_target);
  vps_theta = game.coords[0].theta * UNITS_VPS_TO_DEG;

  vps_daemon_has_run = true;

  release(&vps_data_lock);
}


int vps_data_daemon() {
  while(1) {
    pause(20); // safety interval (arbitrary time)
    vps_download_info();
  }

  return 0;
}

void vps_data_daemon_init() {
  init_lock(&vps_data_lock, "vps_data_lock");
  create_thread(vps_data_daemon, STACK_DEFAULT, 0, "vps_daemon");
}


#endif
