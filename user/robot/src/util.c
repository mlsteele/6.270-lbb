#include <math.h>

#include <util.h>

float fclamp(float n, float low, float high) {
  return (n < high) ? ((n > low) ? n : low) : high;
}

float fclampmag(float n, float low, float high) {
  return n > 0 ? fclamp(n, low, high) : -fclamp(-n, low, high);
}

// smallest angle difference between two angles in degrees
// angle from b to a
float ang_diff(float a, float b) {
  float delta_theta = fmod(a, 360) - fmod(b, 360);
  if (delta_theta < -180) delta_theta += 360;
  if (delta_theta >  180) delta_theta -= 360;
  return delta_theta;
}
