// Include headers from OS
#include <joyos.h>

// usetup is called during the calibration period. 
int usetup (void) {
  return 0;
}

int umain (void) {
  while(1) {
    int16_t vel = frob_read(-255, 255);
    motor_set_vel(0, vel);
    printf("vel: %d    amperage: %d\n", vel, motor_get_current_MA(0));
    pause(120);
  }

  return 0;
}
