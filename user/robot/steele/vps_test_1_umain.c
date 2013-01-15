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
float ang_diff(float a, float b) {
  return fmin(a - b, a - b + 360);
}

// direction 1 for CCW or -1 for CW
void vps_rotate_to_angle(float theta_target, int direction) {
  float max_drive_speed = 0.35;
  float min_drive_speed = 0.12;
  float drive_speed = 0.32;
  float theta_thresh = 7;

  set_wheel_pows(-drive_speed * direction, drive_speed * direction);

  float angdiff = ang_diff(theta_target, get_vps_theta());
  printf("initial angdiff %f\n", angdiff);
  while(angdiff > theta_thresh) {
    // check in with vps
    if (get_us_since_vps_read() > 100000) {
      set_wheel_pows(0,0);
      printf("waiting for vps...");
      wait_for_vps_read();
      printf(" done\n");
    }

    angdiff = ang_diff(theta_target, get_vps_theta());
    printf("loop angdiff %f\n", angdiff);
    print_vps_pos();
    float drive_factor = fmin(angdiff / 90, 1);
    drive_speed = max_drive_speed * drive_factor + min_drive_speed;
    set_wheel_pows(-drive_speed * direction, drive_speed * direction);

    pause(5);
  }

  set_wheel_pows(0, 0);
}

// // direction 1 for forwards or -1 for backwards
// void hybrid_go_straight_by(float mm, int direction) {
//   float drive_speed = 0.4;
//   left_right_float_t start_encs = {encoder_read(PIN_ENCODER_WHEEL_L), encoder_read(PIN_ENCODER_WHEEL_R)};

//   set_wheel_pows(drive_speed * direction, drive_speed * direction);
//   float avg_dist_trav = 0;
//   while(avg_dist_trav < mm) {
//     avg_dist_trav = MM_PER_TICK_WHEELS * ((encoder_read(PIN_ENCODER_WHEEL_L) - start_encs.l) + (encoder_read(PIN_ENCODER_WHEEL_R) - start_encs.r)) / 2;
//     pause(5);
//   }
//   set_wheel_pows(0, 0);
// }

int umain (void) {
  print_vps_pos();

  while(!get_vps_daemon_has_run()) {
    printf("waiting for vps to recv\n");
    pause(500);
  }

  // while(1) {
  //   print_vps_pos();
  //   pause(200);
  // }

  print_vps_pos();

  printf("rotating to angle\n");
  vps_rotate_to_angle(0, 1);
  printf("done\n");

  return 0;
}
