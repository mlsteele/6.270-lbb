// BROKEN

#ifndef _ENCODER_INTEGRATOR_DAEMON_H_
#define _ENCODER_INTEGRATOR_DAEMON_H_

#include <joyos.h>
#include <math.h>
#include <Point.h>
#include <moving.h>

#define ENCODER_INTEGRATION_FREQUENCY_MS 10

struct lock encoder_data_lock;

Point current_guess_pos = {0, 0};
float current_guess_theta = 0;

left_right_float_t enc_int_last_encs = {0, 0};

void flush_encoder_position_integration() {
  left_right_float_t delta_encs = {
    (get_wheel_pows().l > 0 ? 1 : -1) * MM_PER_TICK_WHEELS * (encoder_read(PIN_ENCODER_WHEEL_L) - enc_int_last_encs.l) ,
    (get_wheel_pows().r > 0 ? 1 : -1) * MM_PER_TICK_WHEELS * (encoder_read(PIN_ENCODER_WHEEL_R) - enc_int_last_encs.r) };

  acquire(&encoder_data_lock);

  float avg_delta_encs = (delta_encs.l + delta_encs.r) / 2;
  float distance_traveled_fd_axis = avg_delta_encs * MM_PER_TICK_WHEELS;
  current_guess_pos.x += distance_traveled_fd_axis * cos(current_guess_theta * DEGS_TO_RADS);
  current_guess_pos.y += distance_traveled_fd_axis * sin(current_guess_theta * DEGS_TO_RADS);

  float diff_delta_encs = delta_encs.r - delta_encs.l;
  current_guess_theta += (diff_delta_encs * MM_PER_TICK_WHEELS) / (M_PI * MM_WHEEL_FROM_CENTER) * 360;

  release(&encoder_data_lock);

  enc_int_last_encs.l = encoder_read(PIN_ENCODER_WHEEL_L);
  enc_int_last_encs.r = encoder_read(PIN_ENCODER_WHEEL_R);
}

int encoder_position_daemon() {
  enc_int_last_encs.l = encoder_read(PIN_ENCODER_WHEEL_L);
  enc_int_last_encs.r = encoder_read(PIN_ENCODER_WHEEL_R);

  while(1) {
    flush_encoder_position_integration();
    pause(ENCODER_INTEGRATION_FREQUENCY_MS);
  }

  return 0;
}

void encoder_position_daemon_init() {
  init_lock(&encoder_data_lock, "encoder_data_lock");
  create_thread(encoder_position_daemon, STACK_DEFAULT, 0, "encoder_position_daemon");
}

Point get_encoder_position() {
  acquire(&encoder_data_lock);
  return current_guess_pos;
  release(&encoder_data_lock);
}

float get_encoder_theta() {
  acquire(&encoder_data_lock);
  return current_guess_theta;
  release(&encoder_data_lock);
}

void print_encoder_position() {
  printf("encpos:: <%f, %f> theta_%f\n", get_encoder_position().x, get_encoder_position().y, get_encoder_theta());
}

#endif
