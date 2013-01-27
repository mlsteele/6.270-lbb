#include <transport.h>

#include <util.h>
#include <math.h>
#include <moving.h>
#include <vps_data_daemon.h>
#include <territory.h>

static void aim_towards_target(Point target, float (*theta_accessor)()) {
  float angdiff = ang_diff(points_angle(target, get_vps_position()), theta_accessor());
  float dist = points_distance(target, get_vps_position());

  printf("[2] target < %f, %f >", target.x, target.y);
  printf("  angdiff %f", angdiff);
  printf("  dist %f", dist);

  // fix for orbit bug
  if (fabs(angdiff) > 20 && dist < 200) {
    printf("\n[2] orbit switch\n");
    int direction = angdiff > 0 ? 1 : -1;
    float recover_pow = 0.3;
    set_wheel_pows(-direction * recover_pow, direction * recover_pow);
    return;
  }

  float pow_com = fclamp(dist / 300.f, 0.28, 0.68) * fclamp((50 - fabs(angdiff)) / 50, 0, 1);
  // float pow_bias = fclampmag(pow(angdiff / 90, 3), 0, 0.6);
  float pow_bias = (angdiff > 0 ? 1 : -1) * (0.68 - fclamp(pow_com, 0, 0.68)) / 2;
  set_wheel_pows(
    pow_com - pow_bias ,
    pow_com + pow_bias );
  printf("  pow_com %f", pow_com);
  printf("  pow_bias %f", pow_bias);
  printf("  wheels [%f, %f]\n", get_wheel_pows().l, get_wheel_pows().r);  
}

void aim_towards_target_vps(Point target) {
  aim_towards_target(target, get_vps_theta);
}

void aim_towards_target_vps_gyro(Point target) {
  aim_towards_target(target, gyro_get_degrees);
}

void go_to_territory(int8_t terr_i) {
  terr_i = (6 + terr_i) % 6;
  printf("[3] starting targeting territory %i\n", terr_i);
  while(points_distance(get_vps_position(), territories[terr_i]) > 200) {
    aim_towards_target_vps_gyro(territories[terr_i]);
    pause(6);
    print_vps_pos();
  }
  set_wheel_pows(0,0);
  printf("[3] reached target %i\n", terr_i);
  pause(100);
  bind_gyro_to_vps();
}

void face_towards_gears(int8_t terr_i) {
  pause(100);
  float target_theta = points_angle(gears[terr_i % 6], get_vps_position()) + 180;
  bind_gyro_to_vps();
  rotate_by_gyro(ang_diff(target_theta, get_vps_theta()));
}

void face_towards_mine(int8_t terr_i) {
  terr_i = (6 + terr_i) % 6;
  pause(100);
  float target_theta = points_angle(mines[terr_i], get_vps_position());
  bind_gyro_to_vps();
  rotate_by_gyro(ang_diff(target_theta, get_vps_theta()));
}
