// Periodically updates and converts values from the vps system.
// When using vps data be sure to handle the vps_data_lock

#ifndef _VPS_DATA_DAEMON_H_
#define _VPS_DATA_DAEMON_H_

#include <Point.h>

void vps_data_daemon_init();

Point get_vps_position();
Point get_vps_active_target();
float get_vps_theta();

void print_vps_pos();

#endif
