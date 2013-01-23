#ifndef _TERRITORY_H_
#define _TERRITORY_H_

#include <joyos.h>
#include <Point.h>

// in local coordinates, assume robot starts in center of territory '0' at 0, -y and angle 90
float terr_angles[7];
Point territories[6];
Point gears[6];
Point mines[6];

void territory_init();
void territory_check();
int8_t territory_of_point(Point p);
int8_t current_territory();

void move_to_territory(int8_t territory);
void move_to_next_territory();

#endif
