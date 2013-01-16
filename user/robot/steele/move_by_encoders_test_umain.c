#include <joyos.h>
#include <hw_config.h>
#include <moving.h>

int usetup (void) {
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);

  return 0;
}

int umain (void) {
  printf("moving by encoders...\n");

  // move_distance_by_encoders(304.8);

  for(int i = 0; i < 4; i++) {
    move_distance_by_encoders(304.8);
    rotate_by_encoders(90);
  }

  // set_wheel_pows(-0.1, 0.1);
  // pause(1000);
  // set_wheel_pows(-1, 1);
  // pause(1000);
  // set_wheel_pows(-0.3, 0.3);
  // pause(1000);
  // set_wheel_pows(-0, 0);

  printf("done\n");
  return 0;
}
