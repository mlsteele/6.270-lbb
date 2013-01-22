#include <transport.h>

#include <util.h>
#include <math.h>
#include <moving.h>
#include <vps_data_daemon.h>

void vps_aim_towards_target(Point target, uint8_t dir) {
  // float angdiff = ang_diff(points_angle(target, get_vps_position()) + (dir<0 ? 180 : 0), get_vps_theta());
  float angdiff = ang_diff(points_angle(target, get_vps_position()), get_vps_theta());
  float dist = points_distance(target, get_vps_position());

  printf("target < %f, %f >", target.x, target.y);
  printf("  angdiff %f", angdiff);
  printf("  dist %f", dist);

  // fix for orbit bug
  if (fabs(angdiff) > 20 && dist < 200) {
    printf("  !! orbit switch\n");
    int direction = angdiff > 0 ? 1 : -1;
    float recover_pow = 0.3;
    set_wheel_pows(-direction * recover_pow, direction * recover_pow);
    return;
  }

  float pow_com = fclamp(dist / 300.f, 0.34, 1) * fclamp((40 - fabs(angdiff)) / 40, 0, 1);
  // float pow_bias = fclampmag(pow(angdiff / 90, 3), 0, 0.6);
  float pow_bias = (angdiff > 0 ? 1 : -1) * (1 - pow_com) / 2;
  set_wheel_pows(
    pow_com - pow_bias ,
    pow_com + pow_bias );
  printf("  pow_com %f", pow_com);
  printf("  pow_bias %f", pow_bias);
  printf("  wheels [%f, %f]\n", get_wheel_pows().l, get_wheel_pows().r);
}
