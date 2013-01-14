#include <joyos.h>
#include <hw_config.h>
#include <moving.h>

int usetup (void) {
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);

  return 0;
}

int umain (void) {
  printf("print encs_snapshot\n");
  // encoder_reset(PIN_ENCODER_WHEEL_L);
  // encoder_reset(PIN_ENCODER_WHEEL_R);
  // printf("encs_snapshot [%i : %i]\n", encoder_read(PIN_ENCODER_WHEEL_L), encoder_read(PIN_ENCODER_WHEEL_R));

  printf("power and enter move seq\n");
  float com_pow = 0;
  float pow_targ = 0.6;
  float duration = 1000;
  set_wheel_pows(com_pow, com_pow);
  for (; com_pow < pow_targ; com_pow += pow_targ / (duration / 10)) {
    printf("setting wheels to %f\n", com_pow);
    set_wheel_pows(com_pow, com_pow);
    pause(10);
  }
  for (; com_pow < pow_targ; com_pow += pow_targ / (duration / 10)) {
    printf("setting wheels to %f\n", com_pow);
    set_wheel_pows(com_pow, com_pow);
    pause(10);
  }

  printf("stopping...");
  set_wheel_pows(0, 0);
  printf(" done");
  
  // printf("encs_snapshot [%i : %i]\n", encoder_read(PIN_ENCODER_WHEEL_L), encoder_read(PIN_ENCODER_WHEEL_R));

  return 0;
}
