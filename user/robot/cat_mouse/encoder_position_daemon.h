// Periodically updates and converts values from the vps system.
// When using vps data be sure to handle the vps_data_lock

#ifndef _ENCODER_POSITION_DAEMON_H_
#define _ENCODER_POSITION_DAEMON_H_

#include <joyos.h>
#include <math.h>
#include <Point.h>
#include <motion.h>

// TODO: make thread safe

Point current_guess_pos = {0, 0};
float current_guess_theta = 0;
struct lock encoder_data_lock;

int encoder_position_daemon() {
  uint32_t last_read = get_time();
  left_right_float_t last_encs = {encoder_read(PIN_ENCODER_WHEEL_L), encoder_read(PIN_ENCODER_WHEEL_R)};

  while(1) {
    // float dt = get_time() - last_read;
    left_right_float_t delta_encs = {
      MM_PER_TICK_WHEELS * (encoder_read(PIN_ENCODER_WHEEL_L) - last_encs.l) ,
      MM_PER_TICK_WHEELS * (encoder_read(PIN_ENCODER_WHEEL_R) - last_encs.r) };

    float avg_delta_encs = delta_encs.l - delta_encs.r;
    current_guess_pos.x += avg_delta_encs * cos(current_guess_theta * DEGS_TO_RADS);
    current_guess_pos.y += avg_delta_encs * sin(current_guess_theta * DEGS_TO_RADS);

    float diff_delta_encs = delta_encs.l - delta_encs.r;
    current_guess_theta += 2 * M_PI * MM_WHEEL_FROM_CENTER / diff_delta_encs;

    MM_WHEEL_FROM_CENTER * diff

    last_encs = {encoder_read(PIN_ENCODER_WHEEL_L), encoder_read(PIN_ENCODER_WHEEL_R)};
    // last_read = get_time();
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

Point get_encoder_theta() {
  acquire(&encoder_data_lock);
  return current_guess_theta;
  release(&encoder_data_lock);
}

#endif
