// Include headers from OS
#include <joyos.h>
// #include "../pin_config.h"

// usetup is called during the calibration period. 
int usetup (void) {
  return 0;
}

int umain (void) {
  while(1) {
    int16_t vel = frob_read(-255, 255) - 255;
    motor_set_vel(0, vel);
    pause(0);
    motor_set_vel(1, vel);
    printf("vel: %i    amperage: %i\n", vel, motor_get_current_MA(0));
    pause(120);
  }

  return 0;
}
