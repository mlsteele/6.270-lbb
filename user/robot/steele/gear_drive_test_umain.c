#include <joyos.h>
#include <hw_config.h>
#include <moving.h>

int usetup (void) {
  return 0;
}

int umain (void) {
  pause(200);
  motor_set_vel(PIN_MOTOR_GEAR, 100);
  pause(500);
  motor_set_vel(PIN_MOTOR_GEAR, 255);
  pause(500);
  motor_set_vel(PIN_MOTOR_GEAR, 0);
  pause(500);
  motor_set_vel(PIN_MOTOR_GEAR, -100);
  pause(500);
  motor_set_vel(PIN_MOTOR_GEAR, -255);
  pause(500);
  motor_set_vel(PIN_MOTOR_GEAR, 0);

  return 0;
}
