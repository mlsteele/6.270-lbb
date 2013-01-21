#include <vps_data_daemon.h>

#include <math.h>
#include <hw_config.h>
#include <util.h>

static Point vps_active_target;
static Point vps_position;
static float vps_theta;

static bool vps_daemon_has_run = false;
static struct lock vps_data_lock;

static Point convert_vps_pos_to_mm_pos(board_coord src) {
  Point ret = {src.x * UNITS_VPS_TO_MM, src.y * UNITS_VPS_TO_MM};
  return ret;
}

static bool vps_coords_isnt_zero() {
  int ret = !(  game.coords[0].x     == 0 &&
                game.coords[0].y     == 0 &&
                game.coords[0].theta == 0 );
  // printf("vps_coords_inst_zero: %i\n", ret);
  return ret;
}

static void vps_download_info() {
  // printf("vps_download_info()\n");

  acquire(&vps_data_lock);
  //printf("Starting copy_objects()...\n");
  copy_objects();
  //printf("game.coords: [0]: %d,%d ... [1]: %d,%d\n", game.coords[0].x,game.coords[0].y, game.coords[1].x,game.coords[1].y);

  if (vps_daemon_has_run || vps_coords_isnt_zero()) {
    // printf("swapping vps coords in\n");
    vps_position = convert_vps_pos_to_mm_pos(game.coords[0]);
    vps_active_target = convert_vps_pos_to_mm_pos(game.coords[1]);
    vps_theta = game.coords[0].theta * UNITS_VPS_TO_DEG;
    vps_daemon_has_run = true;
  }
  release(&vps_data_lock);

  // printf("get_vps_last_read_us() -> %i\n", get_vps_last_read_us());
  // print_vps_pos();
}


static int vps_data_daemon() {
  // printf("vps_data_daemon()\n");
  while(1) {
    pause(20); // safety interval (arbitrary time)
    // printf("vps_data_daemon_loop_call\n");
    vps_download_info();
  }

  return 0;
}

void vps_data_daemon_init() {
  init_lock(&vps_data_lock, "vps_data_lock");
  create_thread(vps_data_daemon, STACK_DEFAULT, 0, "vps_daemon");
  // printf("vps_data_daemon_init()\n");
}

// accessors
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

// TODO: fix this

// bool get_vps_daemon_has_run() {
//   acquire(&vps_data_lock);
//   bool ret = vps_daemon_has_run;
//   release(&vps_data_lock);
//   return ret;  
// }

// uint32_t get_vps_last_read_us() {
//   return get_position_microtime();
// }

// float get_vps_us_since_read() {
//   return get_time_us() - get_vps_last_read_us();
// }

// void wait_for_vps_read() {
//   uint32_t last_read_us = get_vps_last_read_us();
//   while(get_vps_last_read_us() == last_read_us) {
//     pause(1);
//   }
// }

// block until a vps reading not older than us microseconds
// UNTESTED
// void ensure_vps_data_newer_than_us(uint32_t us) {
//   // check in with vps
//   if (get_vps_us_since_read() > us) {
//     printf("waiting for vps...");
//     wait_for_vps_read();
//     printf(" done\n");
//   }
// }

void print_vps_pos() {
  Point vpp = get_vps_position();
  printf("<%f, %f>  theta_ %f\n", vpp.x, vpp.y, get_vps_theta());
}
