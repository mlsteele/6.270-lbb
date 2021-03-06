#include <transport.h>

#include <util.h>
#include <hw_config.h>
#include <math.h>
#include <moving.h>
#include <vps_data_daemon.h>
#include <territory.h>

// sets wheels and returns
void aim_towards_target_vps_gyro(Point* target, uint8_t which_forward) {
  Point current_pos = get_vps_position();
  float angdiff;
  if (which_forward == FD_LEVERSIDE) {
    angdiff = ang_diff(points_angle(target, &current_pos), gyro_get_degrees());
  } else {
    angdiff = ang_diff(points_angle(target, &current_pos) + 180, gyro_get_degrees());
  }

  if (which_forward == FD_LEVERSIDE) printf("[2] aimtow: FD_LEVERSIDE\n");
  if (which_forward == FD_SPINSIDE)  printf("[2] aimtow: FD_SPINSIDE\n");

  float dist = points_distance(target, &current_pos);

  printf("[2] target < %f, %f >", target->x, target->y);
  printf("  angdiff %f", angdiff);
  printf("  dist %f", dist);

  // fix for orbit bug
  if (fabs(angdiff) > 20 && dist < 70) {
    printf("\n[2] orbit switch\n");
    int recover_turn_direction = angdiff > 0 ? 1 : -1;
    recover_turn_direction *= (which_forward == FD_LEVERSIDE) ? 1 : -1;
    float recover_pow = 0.3;
    set_wheel_pows(-recover_turn_direction * recover_pow, recover_turn_direction * recover_pow);
    return;
  }

  float pow_com = fclamp(dist / 300.f, 0.28, 0.68) * fclamp((50 - fabs(angdiff)) / 50, 0, 1);
  // float pow_bias = fclampmag(pow(angdiff / 90, 3), 0, 0.6);
  float pow_bias = (angdiff > 0 ? 1 : -1) * (0.68 - fclamp(pow_com, 0, 0.68)) / 2;
  pow_com *= (which_forward == FD_LEVERSIDE) ? 1 : -1;
  set_wheel_pows(
    pow_com - pow_bias ,
    pow_com + pow_bias );
  printf("  pow_com %f", pow_com);
  printf("  pow_bias %f", pow_bias);
  printf("  wheels [%f, %f]\n", get_wheel_pows().l, get_wheel_pows().r);
}

void go_to_point(Point* target, bool initial_rotate) {
  const float close_enough = 200;
  const uint32_t close_enough_time = 250;
  const float close_min = 70;
  const uint32_t gyro_sync_between = 2000;
  Point current_pos = get_vps_position();

  printf("[3] starting targeting point (%.5f, %.5f)\n", target->x, target->y);

  gyro_set_degrees(get_vps_theta());
  // initial rotation
  if (initial_rotate) {
    printf("initial rotation...\n");
    float angdiff_init = ang_diff(points_angle(target, &current_pos), get_vps_theta());
    if (fabs(angdiff_init) < 90) {
      rotate_by_gyro_to(points_angle(target, &current_pos));
    } else {
      rotate_by_gyro_to(points_angle(target, &current_pos) + 180);
    }
    printf("initial rotation done\n");
  }

  gyro_set_degrees(get_vps_theta());
  uint32_t last_gyro_sync = get_time();

  uint32_t close_enough_timer_start = 0;
  bool close_enough_timer_enabled = false;
  while(points_distance(&current_pos, target) > close_min
    && !(close_enough_timer_enabled && get_time() > close_enough_timer_start + close_enough_time)) {
    current_pos = get_vps_position();

    // sync gyro
    if (get_time() > last_gyro_sync + gyro_sync_between) {
      set_wheel_pows(0, 0);
      pause(120);
      gyro_set_degrees(get_vps_theta());
      last_gyro_sync = get_time();
    }

    // escape if close enough for long enough (stuck in seek loop)
    if (!close_enough_timer_enabled && points_distance(&current_pos, target) < close_enough) {
      close_enough_timer_enabled = true;
      close_enough_timer_start = get_time();
    }

    // actuate motors
    float angdiff = ang_diff(points_angle(target, &current_pos), gyro_get_degrees());
    uint8_t which_forward = fabs(angdiff) < 90 ? FD_LEVERSIDE : FD_SPINSIDE;
    aim_towards_target_vps_gyro(target, which_forward);

    pause(6);
    print_vps_pos();
  }
  set_wheel_pows(0,0);
  printf("[3] reached target point (%.5f, %.5f)\n", target->x, target->y);
}

void go_to_territory(int8_t terr_i, bool initial_rotate) {
  terr_i = (terr_i + 6) % 6;
  go_to_point(&territories[terr_i], initial_rotate);
}

void face_towards_gears() {
  Point current_pos = get_vps_position();
  rotate_by_gyro_to(points_angle(&gears[current_territory()], &current_pos) + 180);
}

void face_towards_mine() {
  printf("face_towards_mine()\n");
  Point current_pos = get_vps_position();
  rotate_by_gyro_to(points_angle(&mines[current_territory()], &current_pos));
  printf("face_towards_mine() done\n");
}

void capture_gears(uint8_t color) {
  motor_set_vel(PIN_MOTOR_GEAR, color==TEAM_RED ? -255 : 255);

  gyro_set_degrees(get_vps_theta());
  face_towards_gears();

  gyro_set_degrees(get_vps_theta());
  go_straight_by_gyro_for_ms(900, FD_SPINSIDE);
  set_wheel_pows(-.2, -.2);
  pause(600);
  set_wheel_pows(0, 0);
  pause(500);

  // back out
  set_wheel_pows(0,0);
  gyro_set_degrees(get_vps_theta());
  set_wheel_pows(0.7, 0.7);
  pause(400);
  set_wheel_pows(0, 0);
}

void mine_resources() {
  face_towards_mine();
  set_wheel_pows(0.5, 0.5);
  servo_set_pos(PIN_SERVO_GEARS, SERVO_GEARS_UP);
  pause(1200);
  set_wheel_pows(0,0);
  pause(100);
  gyro_set_degrees(get_vps_theta());
  // contact mine

  for (int i = 0; i < 5; i++) {
    // leve and back up
    servo_set_pos(PIN_SERVO_GEARS, SERVO_GEARS_DN);
    pause(300);
    set_wheel_pows(-0.3, -0.3);
    pause(300);
    set_wheel_pows(0,0);

    // contact mine
    servo_set_pos(PIN_SERVO_GEARS, SERVO_GEARS_UP);
    pause(400);
    set_wheel_pows(0.5, 0.5);
    pause(420);
    set_wheel_pows(0,0);
  }

  // back out
  set_wheel_pows(-0.5, -0.5);
  pause(500);
  set_wheel_pows(0, 0);
}

void dump() {
  // TODO: implement
}
