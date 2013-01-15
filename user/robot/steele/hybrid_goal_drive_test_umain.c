#include <joyos.h>
#include <hw_config.h>
#include <moving.h>
#include <hybrid_integrator_daemon.h>
#include <hybrid_goal_moving.h>

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);

  printf("starting daemons...");
  // vps_data_daemon_init();
  hybrid_position_daemon_init();
  printf(" done\n");

  return 0;
}

int umain (void) {
  hybrid_go_straight_by(100, 1);
  hybrid_orient_at(90, 1);

  return 0;
}
