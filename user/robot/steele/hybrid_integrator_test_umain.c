#include <joyos.h>
#include <hw_config.h>
#include <moving.h>
#include <hybrid_integrator_daemon.h>
#include <vps_data_daemon.h>

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);

  printf("starting daemons...");
  vps_data_daemon_init();
  hybrid_position_daemon_init();
  printf(" done\n");

  return 0;
}

int umain (void) {
  print_hybrid_position();
  set_wheel_pows(0, 0);
  pause(100);
  print_hybrid_position();
  set_wheel_pows(0.43, 0.43);
  pause(500);
  print_hybrid_position();
  set_wheel_pows(-0.43, -0.43);
  pause(500);
  print_hybrid_position();
  set_wheel_pows(-0.43, 0.43);
  pause(500);
  set_wheel_pows(0,0);
  print_hybrid_position();

  return 0;
}
