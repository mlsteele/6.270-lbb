#include <joyos.h>
#include <hw_config.h>
#include <moving.h>

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  // gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);

  return 0;
}

int umain (void) {
  set_wheel_pows(0.5, 0.5);
  pause(1000);
  set_wheel_pows(0, 0);
  pause(200);
  set_wheel_pows(1, 1);
  pause(1000);
  set_wheel_pows(0, 0);

  return 0;
}
