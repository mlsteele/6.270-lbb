#ifndef _TERRITORY_H_
#define _TERRITORY_H_

#include <joyos.h>
#include <Point.h>

float terr_angles[7];
Point territories[6];
Point gears[6];
Point mines[6];

void territory_init();
uint8_t territory_of_point(Point p);
uint8_t current_territory();

void move_to_territory(uint8_t territory);
void move_to_next_territory();

#endif
