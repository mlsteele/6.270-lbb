#ifndef _MOVING_H_
#define _MOVING_H_

#include <util.h>

#define DEFAULT_SPEED .58
#define TARGET_TOLERANCE 50

// smallest angle difference between two angles in degrees
// angle from b to a
float ang_diff(float a, float b);

float get_gyro_current_angle();

l_r_uint16_t get_encoders();

// range [-1,1]
// values out of bounds will be clipped to rangeht
void set_wheel_pows(float l, float r);

l_r_float_t get_wheel_pows();
void print_wheel_pows();

void wheels_brake();

void rotate(float degrees);

float get_heading();

void move_for_time(float velocity, uint32_t millis);

// ~1.2mm overshoot
void move_distance_by_encoders(float distance_mm);

void rotate_by_encoders(float delta_theta);

#endif
