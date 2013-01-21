#include <Point.h>
#include <math.h>

float points_distance(Point a, Point b) {
  return sqrt(pow(a.y-b.y,2)+pow(a.x-b.x,2));
}
