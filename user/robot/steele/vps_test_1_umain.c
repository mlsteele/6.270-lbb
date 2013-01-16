#include <joyos.h>
#include <math.h>
#include <hw_config.h>
#include <moving.h>
#include <vps_data_daemon.h>

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;

  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);

  printf("starting daemons...");
  vps_data_daemon_init();
  printf(" done\n");

  return 0;
}

// smallest angle difference between two angles in degrees
// angle from b to a
float ang_diff(float a, float b) {
  float delta_theta = fmod(a, 360) - fmod(b, 360);
  if (delta_theta < -180) delta_theta += 360;
  if (delta_theta >  180) delta_theta -= 360;
  return delta_theta;
}

float get_angle_between_points(Point src, Point target) {
  Point here = get_vps_position();
  Point here_to_there = {target.x - here.x, target.y - here.y};
  float target_angle = atan2(here_to_there.y, here_to_there.x);
  return target_angle;
}

void vps_rotate_to_angle(float theta_target) {
  float max_drive_speed = 0.75;
  float min_drive_speed = 0.34;
  float theta_thresh = 18;

  float angdiff = ang_diff(theta_target, get_vps_theta());
  printf("initial angdiff %f\n", angdiff);
  while(fabs(angdiff) > theta_thresh) {
    if (get_vps_us_since_read() >= LOST_VPS_US) {
      set_wheel_pows(0, 0);
      printf("catching up with vps...");
      ensure_vps_data_newer_than_us(LOST_VPS_US);
      printf(" done\n");
    }

    angdiff = ang_diff(theta_target, get_vps_theta());

    printf("loop: angdiff[%f]  target[%f]  curang[%f]\n", angdiff, theta_target, get_vps_theta());
    // print_vps_pos();

    float drive_factor = fmin(fabs(angdiff) / 90, 1);
    float drive_speed = max_drive_speed * drive_factor + min_drive_speed;
    int direction = (angdiff > 0) ? 1 : -1;
    set_wheel_pows(- drive_speed * direction, drive_speed * direction);

    pause(5);
  }

  set_wheel_pows(0, 0);
}

void vps_rotate_towards_point(Point target, int direction) {
  ensure_vps_data_newer_than_us(LOST_VPS_US);
  vps_rotate_to_angle(get_angle_between_points(get_vps_position(), get_vps_active_target()));
}

void vps_go_forward_until(bool (*are_we_there_yet) ()) {
  float max_drive_speed = 0.35;
  // float min_drive_speed = 0.12;
  while(!are_we_there_yet()) {
    set_wheel_pows(0, 0);
    ensure_vps_data_newer_than_us(LOST_VPS_US);
    set_wheel_pows(max_drive_speed, max_drive_speed);
  }
  set_wheel_pows(0, 0);
}


// // direction 1 for forwards or -1 for backwards
// void hybrid_go_straight_by(float mm, int direction) {
//   float drive_speed = 0.4;
//   l_r_float_t start_encs = {encoder_read(PIN_ENCODER_WHEEL_L), encoder_read(PIN_ENCODER_WHEEL_R)};

//   set_wheel_pows(drive_speed * direction, drive_speed * direction);
//   float avg_dist_trav = 0;
//   while(avg_dist_trav < mm) {
//     avg_dist_trav = MM_PER_TICK_WHEELS * ((encoder_read(PIN_ENCODER_WHEEL_L) - start_encs.l) + (encoder_read(PIN_ENCODER_WHEEL_R) - start_encs.r)) / 2;
//     pause(5);
//   }
//   set_wheel_pows(0, 0);
// }

bool angle_is_too_far_from_target() {
  return 33 < get_angle_between_points(get_vps_position(), get_vps_active_target());
}


int umain (void) {
  print_vps_pos();

  // while(!get_vps_daemon_has_run()) {
  //   printf("waiting for vps to recv...\n");
  //   pause(500);
  // }

  // printf("waiting for vps read...");
  // wait_for_vps_read();
  // printf(" done\n");

  // print_vps_pos();

  // printf("rotating to angle 0...");
  // vps_rotate_to_angle(0);
  // printf("--> rotate done\n");
  // set_wheel_pows(0, 0);
  // print_vps_pos();
  // pause(500);

  // printf("rotating to angle 90...");
  // vps_rotate_to_angle(90);
  // printf("--> rotate done\n");
  // set_wheel_pows(0, 0);
  // print_vps_pos();
  // pause(500);

  // printf("rotating to angle 180...");
  // vps_rotate_to_angle(180);
  // printf("--> rotate done\n");
  // set_wheel_pows(0, 0);
  // print_vps_pos();
  // pause(500);

  // printf("rotating to angle 270...");
  // vps_rotate_to_angle(270);
  // printf("--> rotate done\n");
  // set_wheel_pows(0, 0);
  // print_vps_pos();
  // pause(500);


  printf("entering section 2\n");

  while(1) {}

  // while(1) {
  //   vps_rotate_towards_point(get_vps_active_target(), DIRECTION_CCW);
  //   vps_go_forward_until(angle_is_too_far_from_target);
  // }

  return 0;
}
