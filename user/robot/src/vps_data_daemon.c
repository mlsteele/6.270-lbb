#include <vps_data_daemon.h>

#include <math.h>
#include <util.h>
#include <hw_config.h>
#include <territory.h>

static Point vps_antagonist;
static Point vps_position;
static float vps_theta;
static uint16_t vps_score;

static bool vps_daemon_has_recvd = false;
static struct lock vps_data_lock;

// static float vps_transform_board_spin = 0; // in degrees
// static float vps_transform_robot_spin = 0; // in degrees
// static uint8_t vps_territory_offset = 0; // index shift for territories

// static Point vps_transform_spin(Point p) {
//   float dtheta = vps_transform_board_spin * DEGS_TO_RADS;
//   return (Point) {
//     (p.x * UNITS_VPS_TO_MM) * cos(dtheta) - (p.y * UNITS_VPS_TO_MM) * sin(dtheta),
//     (p.x * UNITS_VPS_TO_MM) * sin(dtheta) + (p.y * UNITS_VPS_TO_MM) * cos(dtheta)  };
// }

// static float vps_transform_rotate(float theta) {
//   return fmod(theta * UNITS_VPS_TO_DEG + vps_transform_robot_spin, 360);
// }

// prereq: vps has initialized, transform is 0
// robot has not moved
// return success
// static bool vps_init_transform() {
//   float position_tolerance = 200;
//   float angle_tolerance = 30;

//   printf("printing probably untransformed position\n");
//   print_vps_pos();

//   for (int i = 0; i < 6; i++) {
//     float angle = i * 60;
//     Point territory_light_pos = {
//       TERRITORY_RAD_TO_LIGHT * cos(angle * DEGS_TO_RADS) ,
//       TERRITORY_RAD_TO_LIGHT * sin(angle * DEGS_TO_RADS) };
//     if (   points_distance(get_vps_position(), territory_light_pos) < position_tolerance 
//         && fabs(ang_diff(get_vps_theta(), angle + 180)) < angle_tolerance) {
//       vps_transform_board_spin = fmod(1080 - 90 - 60 * i, 360);
//       vps_transform_robot_spin = fmod(1080 - 90 - 60 * i, 360);
//     }
//     vps_territory_offset = 6 - i;
//     printf("vps_init_transform decided guess: [%i] toffset: [%i] board:%f robot:%f\n", i, vps_territory_offset, vps_transform_board_spin, vps_transform_robot_spin);
//     return true;
//   }

//   printf("WARN: vps_init_transform did not match coordinates\n");
//   print_vps_pos();
//   return false;
// }

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

  if (vps_daemon_has_recvd || vps_coords_isnt_zero()) {
    // printf("swapping vps coords in\n");
    vps_position = (Point) {game.coords[0].x * UNITS_VPS_TO_MM, game.coords[0].y * UNITS_VPS_TO_MM};
    vps_antagonist = (Point) {game.coords[1].x * UNITS_VPS_TO_MM, game.coords[1].y * UNITS_VPS_TO_MM};
    vps_theta = game.coords[0].theta * UNITS_VPS_TO_DEG;
    vps_daemon_has_recvd = true;
    vps_score = game.coords[0].score;
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
  printf("initializing vps data daemon...\n");
  init_lock(&vps_data_lock, "vps_data_lock");
  create_thread(vps_data_daemon, STACK_DEFAULT, 0, "vps_daemon");

  printf("waiting for vps first read...\n");
  while(!vps_daemon_has_recvd) pause(30);
  printf("recvd first vps read, printing below\n");
  print_vps_pos();

  // wait for vps transform
  // printf("waiting for vps transform init...\n");
  // while(!vps_init_transform()) pause(30);
  // printf("have set vps transform, applying...\n");
  // vps_download_info(); // immediately update coordinates to reflect transform
  // printf("have applied vps transform\n");
  // print_vps_pos();
}

// accessors
Point get_vps_position() {
  acquire(&vps_data_lock);
  Point ret = vps_position;
  release(&vps_data_lock);
  return ret;
}

Point get_vps_antagonist() {
  acquire(&vps_data_lock);
  Point ret = vps_antagonist;
  release(&vps_data_lock);
  return ret;
}

float get_vps_theta() {
  acquire(&vps_data_lock);
  float ret = vps_theta;
  release(&vps_data_lock);
  return ret;
}

uint16_t get_vps_score() {
  acquire(&vps_data_lock);
  uint16_t ret = vps_score;
  release(&vps_data_lock);
  return ret;
}

// TODO: fix this

// bool get_vps_daemon_has_recvd() {
//   acquire(&vps_data_lock);
//   bool ret = vps_daemon_has_recvd;
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
  printf("[1] <%.5f, %.5f>  theta %.5f\n", vpp.x, vpp.y, get_vps_theta());
}

uint8_t vps_owner(uint8_t terr) {
  acquire(&vps_data_lock);
  uint8_t ret = game.territories[terr].owner;
  release(&vps_data_lock);
  return ret;
}

// uint8_t us_to_vps_numbering(uint8_t terr) {
//   return (terr+vps_territory_offset)%6;
// }

// uint8_t enemy_location() {
//   //TODO fix for real competition
//   //don't forget to return it in our numbering system.
//   acquire(&vps_data_lock);
//   uint8_t ret = 0;
//   release(&vps_data_lock);
//   return ret;
// }

// bool has_balls_remaining(uint8_t terr) {
//   acquire(&vps_data_lock);
//   bool ret = game.territories[us_to_vps_numbering(terr)].remaining > 0;
//   release(&vps_data_lock);
//   return ret;
// }

bool not_over_rate_limit(uint8_t terr) {
  //assumes rate_limit returns milliseconds until territory can be mined
  //TODO verify this assumption
  acquire(&vps_data_lock);
  bool ret = game.territories[terr].rate_limit < 1000;
  release(&vps_data_lock);
  return ret;
}
