#ifndef _POINT_H_
#define _POINT_H_

typedef struct {
  float x;
  float y;
} Point;

float points_distance(Point* a, Point* b);

// angle from b to a
float points_angle(Point* a, Point* b);


#endif
