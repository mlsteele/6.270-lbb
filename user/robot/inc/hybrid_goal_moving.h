#ifndef _HYBRID_GOAL_MOVING_H_
#define _HYBRID_GOAL_MOVING_H_

#include <math.h>
#include <Point.h>

bool hybrid_gyro_within_threshold(float angle, float thresh) {

}

// direction 1 for CC or -1 for CCW
void hybrid_orient_at(float theta, int direction) {
  float drive_speed = 0.32;
  set_wheel_pows(-drive_speed * direction, drive_speed * direction);
  while(!hybrid_gyro_within_threshold(theta, 10)) {
    pause(5);
  }
  set_wheel_pows(0, 0);
}

// direction 1 for forwards or -1 for backwards
void hybrid_go_straight_by(float mm, int direction) {
  float drive_speed = 0.4;
  l_r_float_t start_encs = {encoder_read(PIN_ENCODER_WHEEL_L), encoder_read(PIN_ENCODER_WHEEL_R)};

  set_wheel_pows(drive_speed * direction, drive_speed * direction);
  float avg_dist_trav = 0;
  while(avg_dist_trav < mm) {
    avg_dist_trav = MM_PER_TICK_WHEELS * ((encoder_read(PIN_ENCODER_WHEEL_L) - start_encs.l) + (encoder_read(PIN_ENCODER_WHEEL_R) - start_encs.r)) / 2;
    pause(5);
  }
  set_wheel_pows(0, 0);
}

#endif
