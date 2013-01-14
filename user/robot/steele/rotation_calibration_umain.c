#include <joyos.h>
#include <hw_config.h>
#include <moving.h>

int usetup (void) {
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);

  return 0;
}

int umain (void) {
  float theta_init = gyro_get_degrees();
  float theta_target = theta_init + 360;
  uint32_t start_time_ms = get_time();
  uint32_t timeout_ms = 3000;

  printf("enter turn loop\n");
  set_wheel_pows(-0.5, 0.5);

  while(1) {
    printf("loop step:: gyro_deg: %f    ms: %i\n", gyro_get_degrees(), get_time());
    if (get_time() - start_time_ms > timeout_ms) {
      printf("breaking from timeout\n");
      break;
    }
    
    if (gyro_get_degrees() > theta_target) {
      printf("breaking from degrees\n");
      break;
    }
  }

  printf("stopping...");
  set_wheel_pows(0,0);
  printf(" done\n");

  pause(1000);

  while(1) {
    printf("gyro_deg: %f\n", gyro_get_degrees());
    pause(50);
  }

  return 0;
}
