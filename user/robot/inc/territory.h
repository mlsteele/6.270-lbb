#ifndef _TERRITORY_H_
#define _TERRITORY_H_

#include <joyos.h>
#include <Point.h>

// in local coordinates, assume robot starts in center of territory '0' at 0, -y and angle 90
Point territories[6];
Point gears[6];
Point pre_gears[6];
Point mines[6];
Point pre_mines[6];
float territory_angle_mid[6]; // bounding angles of territories
float territory_angle_cw[6]; // bounding angles of territories
float territory_angle_ccw[6];

void territory_init(); // initialize positions
int8_t territory_of_point(Point p);
int8_t current_territory();

void move_to_territory(int8_t territory);
void move_to_next_territory();
void move_to_next_territory_dr();
void move_to_territory_dr(uint8_t terr);
void move_to_gear();
void spin_gear();
#endif
