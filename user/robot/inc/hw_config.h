// hardware configuration and common conversion factors

#ifndef _HW_CONFIG_H_
#define _HW_CONFIG_H_

// PINS
#define PIN_MOTOR_DRIVE_L 0
#define PIN_MOTOR_DRIVE_R 1
#define PIN_GYRO 21

#define PIN_ENCODER_WHEEL_L 27
#define PIN_ENCODER_WHEEL_R 26


// PHYSICAL
// this one is a lie
// #define MM_WHEEL_FROM_CENTER 81.5
// this one is the truth
// #define MM_WHEEL_FROM_CENTER 97.79
// this one is a lie, but a good lie
#define MM_WHEEL_FROM_CENTER 105.4
// 32 inches in ~470 ticks
#define MM_PER_TICK_WHEELS 1.7293617
#define LSB_US_PER_DEG 1386583

// CONVERSION
#define DEGS_TO_RADS 0.01745328f

#define UNITS_VPS_TO_MM 0.5953
#define UNITS_VPS_TO_DEG 0.08789


// COMMON

#define DIRECTION_CW -1
#define DIRECTION_CCW 1
#define LOST_VPS_US 200000

typedef struct {
  float r;
  float l;
} left_right_float_t;

float fclamp(float n, float low, float high) {
  return (n < high) ? ((n > low) ? n : low) : high;
}

#endif
