// Periodically updates and converts values from the vps system.
// When using vps data be sure to handle the vps_data_lock

#ifndef _VPS_DATA_DAEMON_H_
#define _VPS_DATA_DAEMON_H_

#include <joyos.h>
#include <hw_config.h>
#include <Point.h>

Point vps_active_target;
Point vps_position;
float vps_theta;

bool vps_daemon_has_run = false;
struct lock vps_data_lock;

Point convert_vps_pos_to_mm_pos(board_coord src) {
  Point ret = {src.x * UNITS_VPS_TO_MM, src.y * UNITS_VPS_TO_MM};
  return ret;
}

bool vps_coords_isnt_zero() {
  return !( game.coords[0].x     == 0 &&
            game.coords[0].y     == 0 &&
            game.coords[0].theta == 0 );
}

void vps_download_info() {
  acquire(&vps_data_lock);
  //printf("Starting copy_objects()...\n");
  copy_objects();
  //printf("game.coords: [0]: %d,%d ... [1]: %d,%d\n", game.coords[0].x,game.coords[0].y, game.coords[1].x,game.coords[1].y);

  if(vps_coords_isnt_zero() || vps_daemon_has_run) {
    vps_position = convert_vps_pos_to_mm_pos(game.coords[0]);
    vps_active_target = convert_vps_pos_to_mm_pos(game.coords[1]);
    vps_theta = game.coords[0].theta * UNITS_VPS_TO_DEG;
    vps_daemon_has_run = true;
  }
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

//Accessors!
Point get_vps_position() {
  acquire(&vps_data_lock);
  Point ret = vps_position;
  release(&vps_data_lock);
  return ret;
}

Point get_vps_active_target() {
  acquire(&vps_data_lock);
  Point ret = vps_active_target;
  release(&vps_data_lock);
  return ret;
}

float get_vps_theta() {
  acquire(&vps_data_lock);
  float ret = vps_theta;
  release(&vps_data_lock);
  return ret;
}

bool get_vps_daemon_has_run() {
  acquire(&vps_data_lock);
  bool ret = vps_daemon_has_run;
  release(&vps_data_lock);
  return ret;  
}

float get_vps_last_read_us() {
  return get_position_microtime();
}

float get_us_since_vps_read() {
  return get_time_us() - get_vps_last_read_us();
}

void wait_for_vps_read() {
  uint32_t last_read_us = get_vps_last_read_us();
  while(get_vps_last_read_us() == last_read_us) {
    pause(1);
  }
}

#endif
