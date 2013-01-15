#include <joyos.h>
#include <hw_config.h>
#include <moving.h>

int usetup (void) {
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);

  return 0;
}

bool are_we_there_yet() {
  float enc_avg = (encoder_read(PIN_ENCODER_WHEEL_L) + encoder_read(PIN_ENCODER_WHEEL_R)) / 2;
  return enc_avg > 304.8 / MM_PER_TICK_WHEELS;
}

int umain (void) {
  printf("print encs_snapshot\n");
  encoder_reset(PIN_ENCODER_WHEEL_L);
  encoder_reset(PIN_ENCODER_WHEEL_R);
  printf("encs_snapshot [%i : %i]\n", encoder_read(PIN_ENCODER_WHEEL_L), encoder_read(PIN_ENCODER_WHEEL_R));

  printf("move seq\n");

  set_wheel_pows(0.4, 0.4);

  while(!are_we_there_yet()) {}

  printf("stopping...");
  set_wheel_pows(0, 0);
  printf(" done\n");

  printf("encs_snapshot [%i : %i]\n", encoder_read(PIN_ENCODER_WHEEL_L), encoder_read(PIN_ENCODER_WHEEL_R));

  return 0;
}
