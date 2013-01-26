#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <moving.h>
#include <math.h>
#include "get_vps.h"
#include "vps_utils.h"

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;

  return 0;
}

int umain (void) {
  // 2-step rotate and go version
	// while(1) {
	// 	Point target = get_vps_target();
	// 	printf("Got new target (%.2f, %.2f)\n", target.x, target.y);
	// 	vps_rotate(target_theta(target));
	// 	vps_drive_towards(target);
	// }

  // proportional control version
  while(1) {
    aim_towards_target_vps();
    pause(4);
  }
  return 0;
}
