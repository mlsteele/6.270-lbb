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
  rotate(30);
  for (int i = 0; i < 6; i++) {
    move_distance_by_encoders(points_distance(territories[0], territories[1]));
    rotate_by_gyro(60);
  }

  set_wheel_pows(0, 0);

  return 0;
}
