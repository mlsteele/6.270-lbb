// BROKEN

#ifndef _HYBRID_INTEGRATOR_DAEMON_H_
#define _HYBRID_INTEGRATOR_DAEMON_H_

#include <joyos.h>
#include <math.h>
#include <Point.h>
#include <moving.h>

#define HYBRID_INTEGRATION_FREQUENCY_MS 10

struct lock hybrid_integrator_data_lock;

Point hybrid_int_pos = {0, 0};

l_r_uint16_t hybrid_int_last_encs = {0, 0};

void flush_hybrid_position_integration() {
  l_r_uint16_t encoders = get_encoders();
  l_r_uint16_t delta_encs_mm = {
    (get_wheel_pows().l<0 ? -1 : 1) * (encoders.l - hybrid_int_last_encs.l) ,
    (get_wheel_pows().r<0 ? -1 : 1) * (encoders.r - hybrid_int_last_encs.r) };

  hybrid_int_last_encs = encoders;

  acquire(&hybrid_integrator_data_lock);

  float avg_delta_encs_mm = (delta_encs_mm.l + delta_encs_mm.r) / 2;
  float distance_traveled_fd_axis = avg_delta_encs_mm * MM_PER_TICK_WHEELS;
  hybrid_int_pos.x += distance_traveled_fd_axis * cos(gyro_get_degrees() * DEGS_TO_RADS);
  hybrid_int_pos.y += distance_traveled_fd_axis * sin(gyro_get_degrees() * DEGS_TO_RADS);

  release(&hybrid_integrator_data_lock);
}

int hybrid_position_daemon() {
  hybrid_int_last_encs = get_encoders();

  while(1) {
    flush_hybrid_position_integration();
    pause(HYBRID_INTEGRATION_FREQUENCY_MS);
  }

  return 0;
}

void hybrid_position_daemon_init() {
  init_lock(&hybrid_integrator_data_lock, "hybrid_integrator_data_lock");
  create_thread(hybrid_position_daemon, STACK_DEFAULT, 0, "encoder_position_daemon");
}

Point get_hybrid_position() {
  acquire(&hybrid_integrator_data_lock);
  return hybrid_int_pos;
  release(&hybrid_integrator_data_lock);
}

void print_hybrid_position() {
  printf("encpos:: <%f, %f> theta_%f\n", get_hybrid_position().x, get_hybrid_position().y, gyro_get_degrees());
}

#endif
