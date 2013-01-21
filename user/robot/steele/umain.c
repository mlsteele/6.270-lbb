#include <joyos.h>
#include <hw_config.h>
#include <moving.h>
#include <vps_data_daemon.h>

int usetup() {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);
  vps_data_daemon_init();

  return 0;
}

int umain() {
  printf("reached umain\n");
  while(1) {
    print_vps_pos();
    pause(400);
  }

  set_wheel_pows(0.3, 0.3);
  pause(100);
  set_wheel_pows(0, 0);

  return 0;
}
