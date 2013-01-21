#include <vps_data_daemon.h>

#include <math.h>
#include <hw_config.h>
#include <util.h>

static Point vps_active_target;
static Point vps_position;
static float vps_theta;

static bool vps_daemon_has_run = false;
static struct lock vps_data_lock;

static float vps_transform_board_spin = 0; // in degrees
static float vps_transform_robot_spin = 0; // in degrees
static uint8_t vps_territory_offset = 0; // index shift for territories

static Point vps_transform_spin(Point p) {
  float vps_transform_board_spin_radians = vps_transform_board_spin * DEGS_TO_RADS;
  return (Point) {
    (p.x * UNITS_VPS_TO_MM) * cos(vps_transform_board_spin_radians) - (p.y * UNITS_VPS_TO_MM) * sin(vps_transform_board_spin_radians),
    (p.x * UNITS_VPS_TO_MM) * sin(vps_transform_board_spin_radians) + (p.y * UNITS_VPS_TO_MM) * cos(vps_transform_board_spin_radians)  };
}

static float vps_transform_rotate(float theta) {
  return fmod(theta * UNITS_VPS_TO_DEG + vps_transform_robot_spin, 360);
}

// prereq: vps has initialized, transform is 0
// robot has not moved
// return success
static bool vps_init_transform() {
  float position_tolerance = 400;
  float angle_tolerance = 40;

  printf("printing probably untransformed position\n");
  print_vps_pos();

  if (points_distance(get_vps_position(), (Point){-858, 0}) < position_tolerance && fabs(ang_diff(get_vps_theta(), 0)) < angle_tolerance) {
    vps_transform_board_spin = 0;
    vps_transform_robot_spin = 0;
    printf("vps_init_transform decided board:%f robot:%f\n", vps_transform_board_spin, vps_transform_robot_spin);
    vps_territory_offset = 0;
    return true;
  } else if (points_distance(get_vps_position(), (Point){858, 0}) < position_tolerance && fabs(ang_diff(get_vps_theta(), 180)) < angle_tolerance) {
    vps_transform_board_spin = 180;
    vps_transform_robot_spin = 180;
    printf("vps_init_transform decided board:%f robot:%f\n", vps_transform_board_spin, vps_transform_robot_spin);
    vps_territory_offset = 3;
    return true;
  } else {
    printf("WARN: vps_init_transform felt indecisive\n");
    print_vps_pos();
    return false;
  }
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
    vps_position = vps_transform_spin((Point) {game.coords[0].x, game.coords[0].y});
    vps_active_target = vps_transform_spin((Point) {game.coords[1].x, game.coords[1].y});
    vps_theta = vps_transform_rotate(game.coords[0].theta);
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

  printf("waiting for vps first read...\n");
  while(!vps_daemon_has_run) pause(30);
  printf("recvd first vps read, printing below\n");
  print_vps_pos();

  // wait for vps transform
  printf("waiting for vps transform init...\n");
  while(!vps_init_transform()) pause(30);
  printf("have set vps transform, applying...\n");
  vps_download_info(); // immediately update coordinates to reflect transform
  printf("have applied vps transform\n");
  print_vps_pos();
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

uint8_t vps_owner(uint8_t terr) {
  acquire(&vps_data_lock);
  uint8_t ret = game.territories[us_to_vps_numbering(terr)].owner;
  release(&vps_data_lock);
  return ret;
}

uint8_t us_to_vps_numbering(uint8_t terr) {
  return (terr+vps_territory_offset)%6;
}

uint8_t enemy_location() {
  //TODO fix for real competition
  //don't forget to return it in our numbering system.
  acquire(&vps_data_lock);
  uint8_t ret = 0;
  release(&vps_data_lock);
  return ret;
}

bool has_balls_remaining(uint8_t terr) {
  acquire(&vps_data_lock);
  bool ret = game.territories[us_to_vps_numbering(terr)].remaining > 0;
  release(&vps_data_lock);
  return ret;
}

bool not_over_rate_limit(uint8_t terr) {
  //assumes rate_limit returns milliseconds until territory can be mined
  //TODO verify this assumption
  acquire(&vps_data_lock);
  bool ret = game.territories[us_to_vps_numbering(terr)].rate_limit < 1000;
  release(&vps_data_lock);
  return ret;
}
