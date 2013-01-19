// BROKEN

#ifndef _ENCODER_INTEGRATOR_DAEMON_H_
#define _ENCODER_INTEGRATOR_DAEMON_H_

#include <Point.h>

void encoder_integrator_daemon_init();

Point get_encoder_position();
float get_encoder_theta();

void print_encoder_position();

#endif
