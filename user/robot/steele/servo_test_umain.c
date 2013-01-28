#include <joyos.h>
#include <hw_config.h>

int usetup (void) {
  return 0;
}

int umain (void) {
  // pass through frob knob
  // while(1) {
  //   int val = frob_read_range(0, 511);
  //   printf("setting servo to %i\n", val);
  //   servo_set_pos(PIN_SERVO_GEARS, val);
  // }

  servo_set_pos(PIN_SERVO_GEARS, SERVO_GEARS_UP);
  pause(500);
  for (int i = 0; i < 5; i++) {
    servo_set_pos(PIN_SERVO_GEARS, SERVO_GEARS_DN);
    pause(1000);
    servo_set_pos(PIN_SERVO_GEARS, SERVO_GEARS_UP);
    pause(500);
  }

  servo_disable(PIN_SERVO_GEARS);
  return 0;
}
