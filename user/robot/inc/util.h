#ifndef _ROBOT_UTIL_H_
#define _ROBOT_UTIL_H_

#include <joyos.h>

typedef struct {
  float r;
  float l;
} l_r_float_t;

typedef struct {
  uint16_t r;
  uint16_t l;
} l_r_uint16_t;

float fclamp(float n, float low, float high);

// smallest angle difference between two angles in degrees
// angle from b to a
float ang_diff(float a, float b);

#endif
