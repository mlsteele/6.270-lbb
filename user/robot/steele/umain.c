#include <joyos.h>
#include <hw_config.h>
#include <moving.h>
#include <vps_data_daemon.h>
#include <territory.h>

int usetup() {
  extern volatile uint8_t robot_id;
  robot_id = 8;

  territory_init();
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);
  return 0;
}

int umain() {
  rotate_by_gyro(-30);
  set_wheel_pows(0,0);
  pause(500);
  for (int i = 0; i < 6; i++) {
    move_distance_by_encoders_gyro(points_distance(territories[0], territories[1]));
    set_wheel_pows(0,0);
    pause(500);
    rotate_by_gyro(60);
  }

  // const int pows_len = 5;
  // float pows[5] = {0.3, 0.6, 1, 0.5, 0.2};

  // for (int i = 0; i < pows_len; i++) {
  //   set_wheel_pows(pows[i], pows[i]);
  //   pause(500);
  // }

  set_wheel_pows(0, 0);

  return 0;
}
