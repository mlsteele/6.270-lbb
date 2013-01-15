#include <joyos.h>
#include <hw_config.h>
#include <moving.h>

int usetup() {
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);

  return 0;
}

float read_rotational_encoding() {
  return (encoder_read(PIN_ENCODER_WHEEL_L) + encoder_read(PIN_ENCODER_WHEEL_R)) * MM_PER_TICK_WHEELS;
}

int umain() {
  encoder_reset(PIN_ENCODER_WHEEL_L);
  encoder_reset(PIN_ENCODER_WHEEL_R);
  float start_gyro = gyro_get_degrees();

  set_wheel_pows(0.3, -0.3);
  while(read_rotational_encoding() < 2.f * M_PI * MM_WHEEL_FROM_CENTER) {
    printf("cirtr: %f\n", read_rotational_encoding());
  }

  printf("stopping...");
  set_wheel_pows(0, 0);
  printf(" done\n");  

  printf("%f\n", gyro_get_degrees() - start_gyro);
}
