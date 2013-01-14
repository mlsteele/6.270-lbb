#ifndef _NAVIGATION_H_
#define _NAVIGATION_H_

#include <joyos.h>
#include <motor.h>
#include <math.h>
#include "vector_tools.h"

// TODO: this does not belong
v2d robot_position_estimate() {
  v2d v = {0,0};
  return v;
}


struct lock nav_lock;

bool target_pos_active = false;
v2d target_pos;

void set_wheels(float vl, float vr) {
  motor_set_vel(PIN_MOTOR_DRIVE_L, vr);
  motor_set_vel(PIN_MOTOR_DRIVE_R, vl);
}

void move_robot_to(v2d pos) {
  v2d rpe = robot_position_estimate();
  v2d diff = v2d_sub(&pos, &rpe); // FIXME: pointer issues
  float ang = fmod(atan2(diff.y, diff.x) + 2 * M_PI, 2 * M_PI); // radians

  float motor_vels[2] = {0,0}; // range: [-1, 1]

  // rotation component
  motor_vels[0] += ang / 2 / M_PI / 4;
  motor_vels[1] -= ang / 2 / M_PI / 4;

  // straight componenet
  static float charge_angle_thresh = 0.523598776; // radians
  if (abs(ang) < charge_angle_thresh) {
    motor_vels[0] += 0.5;
    motor_vels[1] += 0.5;
  }

  // legalize motor vel range
  motor_vels[0] = motor_vels[0] * 255;
  printf("setting wheels powers : [%f, %f]\n", motor_vels[0], motor_vels[1]);
  set_wheels(fmin(fmax(-1, motor_vels[0]), 1) * 255, fmin(fmax(-1, motor_vels[1]), 1) * 255);
}

int navigation_controller() {
  while(1) {
    acquire(&nav_lock);
    if (target_pos_active) {
      move_robot_to(target_pos);
    }
    pause(50); // safety interval (arbitrary time)
    release(&nav_lock);
  }

  return 0;
}

// pass null to disable position targeting
void set_target_pos(v2d* pos) {
  printf("nav: set_target_pos   (is_null==%d)", pos == NULL);

  acquire(&nav_lock);
  if (pos != NULL)
    target_pos = *pos;
  target_pos_active = (pos != NULL);
  release(&nav_lock);
}


#endif
