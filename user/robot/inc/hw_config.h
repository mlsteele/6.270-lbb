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
#warning these physical constant definitions are notoriously uninformed
#define MM_WHEEL_FROM_CENTER 50
#define MM_PER_TICK_WHEELS 4
#define LSB_US_PER_DEG 1386583

// CONVERSION
#define DEGS_TO_RADS 0.01745328f

#endif
