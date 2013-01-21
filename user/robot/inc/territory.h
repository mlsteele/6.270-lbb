#ifndef _TERRITORY_H_
#define _TERRITORY_H_

#include <joyos.h>
#include <Point.h>
#include <vps_data_daemon.h>

// read only for user (please)
typedef struct territory_ {
  uint8_t id;

  // accessors for threaded data
  uint8_t (*n_balls)();
  uint8_t (*owner_id)();

  // threaded data, do not touch!
  uint8_t _n_balls;
  uint8_t _owner_id;

  // float theta_cw;
  // float theta_ccw;

  // Point center_pos;
  // Point dispenser;
  // Point capture;

  struct territory_* cw;
  struct territory_* ccw;
} territory;

#endif
