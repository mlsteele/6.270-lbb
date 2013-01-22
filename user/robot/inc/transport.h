#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_

#include <joyos.h>
#include <Point.h>

void vps_aim_towards_target(Point target, uint8_t dir);
void vps_gyro_aim_towards_target(Point target, uint8_t dir);

#endif
