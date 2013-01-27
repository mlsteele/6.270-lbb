// level 2 movement

#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

#include <joyos.h>
#include <Point.h>

void aim_towards_target_vps(Point target);
void aim_towards_target_vps_gyro(Point target);
void go_to_point(Point target);
void go_to_territory(int8_t terr_i);
void face_towards_gears();
void face_towards_mine();
void capture_gears();

#endif
