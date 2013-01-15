#include <joyos.h>
#include <hw_config.h>
#include <moving.h>

int usetup (void) {
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);

  return 0;
}

int umain (void) {
  printf("print encs_snapshot\n");

  float start_angle = get_gyro_current_angle();

  printf("begin follow loop\n");
  while(1) {
    float theta_offset = start_angle - get_gyro_current_angle();
    float wheel_bias = theta_offset / 90;
    float pow_base = 0.3;
    set_wheel_pows(pow_base - wheel_bias, pow_base + wheel_bias);
    printf("m_l: %f", get_wheel_pows().l);
    printf("    m_l: %f", get_wheel_pows().r);
    printf("    theta_offset: %f", theta_offset);
    printf("    wheel_bias: %f", wheel_bias);
    printf("\n");
  }

  return 0;
}
