// hardware configuration and common conversion factors

#ifndef _HW_CONFIG_H_
#define _HW_CONFIG_H_

// PINS
#define PIN_MOTOR_DRIVE_L 0
#define PIN_MOTOR_DRIVE_R 1
#define PIN_GYRO 21

#define PIN_ENCODER_WHEEL_L 26
#define PIN_ENCODER_WHEEL_R 27


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

// BOARD
#define TERRITORY_RAD_TO_LIGHT 858
#define TERRITORY_RAD_TO_GEARS 1243
#define TERRITORY_RAD_TO_MINE 1249

// CONVERSION
#define DEGS_TO_RADS 0.01745328f
#define RADS_TO_DEGS 57.295820614f

#define UNITS_VPS_TO_MM 0.5953
#define UNITS_VPS_TO_DEG 0.08789


// COMMON
#define DIRECTION_CW -1
#define DIRECTION_CCW 1
#define LOST_VPS_US 200000

#endif
