#include <joyos.h>
#include <hw_config.h>
#include <moving.h>

int usetup (void) {
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);

  return 0;
}

int umain (void) {
  printf("testing wheels forward...\n");
  set_wheel_pows(0.3, 0.3);
  pause(50);
  set_wheel_pows(0, 0);
  printf("done...\n");

  printf("preparing to print sensors...\n");

  while(1) {
    printf("gyro: %f", gyro_get_degrees());
    printf("  enc_l: %i", encoder_read(PIN_ENCODER_WHEEL_L));
    printf("  enc_r: %i", encoder_read(PIN_ENCODER_WHEEL_R));
    printf("\n");
    pause(50);
  }

  return 0;
}
