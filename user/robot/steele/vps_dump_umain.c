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

void print_vps_pos() {
  Point vpp = get_vps_position();
  printf("<%f, %f>  theta_ %f\n", vpp.x, vpp.y, get_vps_theta());
}

int umain (void) {
  while(!get_vps_daemon_has_run()) {
    printf("waiting for vps to recv\n");
    pause(500);
  }

  while(1) {
    print_vps_pos();
    pause(200);
  }

  return 0;
}
