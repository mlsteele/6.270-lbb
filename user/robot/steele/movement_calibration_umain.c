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
  printf("encs_snapshot [%i : %i]\n", encoder_read(PIN_ENCODER_WHEEL_L), encoder_read(PIN_ENCODER_WHEEL_R));

  printf("move seq\n");

  // float com_pow = 0;
  // float pow_targ = 0.6;
  // float duration = 1000;
  // set_wheel_pows(com_pow, com_pow);
  // for (; com_pow < pow_targ; com_pow += pow_targ / (duration / 10)) {
  //   printf("setting wheels to %f\n", com_pow);
  //   set_wheel_pows(com_pow, com_pow);
  //   pause(10);
  // }
  // pause(30);
  // com_pow = pow_targ;
  // for (; com_pow > 0; com_pow -= pow_targ / (duration / 10)) {
  //   printf("setting wheels to %f\n", com_pow);
  //   set_wheel_pows(com_pow, com_pow);
  //   pause(10);
  // }

  int pow_chain_len = 11;
  float pow_chain[11] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1};

  for(int i = 0; i < pow_chain_len; i++) {
    printf("encs_snapshot [%i : %i]\n", encoder_read(PIN_ENCODER_WHEEL_L), encoder_read(PIN_ENCODER_WHEEL_R));
    printf("setting wheel pows to %f\n", pow_chain[i]);
    set_wheel_pows(pow_chain[i], pow_chain[i]);
    pause(250);
  }

  printf("stopping...");
  set_wheel_pows(0, 0);
  printf(" done");
  
  printf("encs_snapshot [%i : %i]\n", encoder_read(PIN_ENCODER_WHEEL_L), encoder_read(PIN_ENCODER_WHEEL_R));

  return 0;
}
