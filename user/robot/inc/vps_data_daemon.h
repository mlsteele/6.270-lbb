// Periodically updates and converts values from the vps system.
// When using vps data be sure to handle the vps_data_lock

#ifndef _VPS_DATA_DAEMON_H_
#define _VPS_DATA_DAEMON_H_

#include <joyos.h>
#include <Point.h>

// A note to the wary:
// this blocks until there is a vps reading,
// and then it block some more until the robot is near a valid starting location
// valid starting locations are one of two points on the board across the x axis.
void vps_data_daemon_init();

Point get_vps_position();
Point get_vps_antagonist();
float get_vps_theta();
void print_vps_pos();

// uint8_t vps_owner(uint8_t territory);
// uint8_t us_to_vps_numbering(uint8_t terr);
// uint8_t enemy_location();
// bool has_balls_remaining(uint8_t terr);
// bool not_over_rate_limit(uint8_t terr);

#endif
