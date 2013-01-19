// tracks position of robot using encoders
// UNTESTED

#include <joyos.h>
#include <math.h>
#include <moving.h>
#include <hw_config.h>

#include <encoder_integrator_daemon.h>

#define ENCODER_INTEGRATION_FREQUENCY_MS 10

static struct lock encoder_integrator_daemon_lock;

static Point guess_pos = {0, 0};
static float guess_theta = 0;

static l_r_float_t last_encs = {0, 0};

static void flush_integration() {
  // printf("encoder_integrator::flush_integration()\n");
  l_r_float_t delta_encs = {
    (get_wheel_pows().l > 0 ? 1 : -1) * (encoder_read(PIN_ENCODER_WHEEL_L) - last_encs.l) ,
    (get_wheel_pows().r > 0 ? 1 : -1) * (encoder_read(PIN_ENCODER_WHEEL_R) - last_encs.r) };

  // printf("delta encs == [%f, %f]\n", delta_encs.l, delta_encs.r);

  acquire(&encoder_integrator_daemon_lock);

  float avg_delta_encs = (delta_encs.l + delta_encs.r) / 2;
  float distance_traveled_fd_axis = avg_delta_encs * MM_PER_TICK_WHEELS;
  guess_pos.x += distance_traveled_fd_axis * cos(guess_theta * DEGS_TO_RADS);
  guess_pos.y += distance_traveled_fd_axis * sin(guess_theta * DEGS_TO_RADS);

  float diff_delta_encs = delta_encs.r - delta_encs.l;
  guess_theta += (diff_delta_encs * MM_PER_TICK_WHEELS) / (M_PI * MM_WHEEL_FROM_CENTER) * 90;

  release(&encoder_integrator_daemon_lock);

  last_encs.l = encoder_read(PIN_ENCODER_WHEEL_L);
  last_encs.r = encoder_read(PIN_ENCODER_WHEEL_R);
}

static int encoder_integrator_daemon() {
  last_encs.l = encoder_read(PIN_ENCODER_WHEEL_L);
  last_encs.r = encoder_read(PIN_ENCODER_WHEEL_R);

  while(1) {
    flush_integration();
    pause(ENCODER_INTEGRATION_FREQUENCY_MS);
  }

  return 0;
}

void encoder_integrator_daemon_init() {
  init_lock(&encoder_integrator_daemon_lock, "encoder_integrator_daemon_lock");
  create_thread(encoder_integrator_daemon, STACK_DEFAULT, 0, "encoder_integrator_daemon");
}

Point get_encoder_position() {
  acquire(&encoder_integrator_daemon_lock);
  Point ret = guess_pos;
  release(&encoder_integrator_daemon_lock);
  return ret;
}

float get_encoder_theta() {
  acquire(&encoder_integrator_daemon_lock);
  float ret = guess_theta;
  release(&encoder_integrator_daemon_lock);
  return ret;
}

void print_encoder_position() {
  printf("encpos == < %f , %f > theta %f\n", get_encoder_position().x, get_encoder_position().y, get_encoder_theta());
}
