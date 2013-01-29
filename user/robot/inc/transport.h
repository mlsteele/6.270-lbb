// level 2 movement

#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

#include <joyos.h>
#include <Point.h>

// which_foward is one of FD_LEVERSIDE or FD_GEARSIDE
void aim_towards_target_vps_gyro(Point* target, uint8_t which_forward);
void go_to_point(Point* target, bool initial_rotate);
void go_to_territory(int8_t terr_i, bool initial_rotate);
void face_towards_gears();
void face_towards_mine();
void capture_gears();
void mine_resources();
void dump();

#endif
