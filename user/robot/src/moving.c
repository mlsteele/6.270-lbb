#include <math.h>
#include <hw_config.h>
#include <Point.h>
// #include <gyro.h>
#include <vps_data_daemon.h>

#include <moving.h>

static l_r_float_t last_motor_pows = {0, 0};

l_r_uint16_t get_encoders() {
  l_r_uint16_t encs;
  encs.l = encoder_read(PIN_ENCODER_WHEEL_L);
  encs.r = encoder_read(PIN_ENCODER_WHEEL_R);
  return encs;
}

// range [-1,1]
// values out of bounds will be clipped to rangeht
void set_wheel_pows(float l, float r) {
  // printf("set wheel pows to %f and %f\n", l, r);
  motor_set_vel(PIN_MOTOR_DRIVE_L, l >= -1 ? (l <= 1 ? (l * 255) : 255) : -255);
  motor_set_vel(PIN_MOTOR_DRIVE_R, r >= -1 ? (r <= 1 ? (r * 255) : 255) : -255);

  // flush_hybrid_position_integration();
  // flush_encoder_position_integration();

  // cache vals
  last_motor_pows.l = l;
  last_motor_pows.r = r;
}

l_r_float_t get_wheel_pows() { return last_motor_pows; }
void print_wheel_pows() { printf("wheel_pows [%f, %f]\n", get_wheel_pows().l, get_wheel_pows().r); }

void wheels_brake() {
  set_wheel_pows(0, 0);
}

// void rotate(float degrees) {
//   printf("Rotating...\n");
//   printf("start angle = %f \n", gyro_get_degrees());
//   float desired_angle = gyro_get_degrees() + degrees;
//   printf("desired_angle = %f \n", desired_angle);
//
//   // NOTE: (miles) removed the no-op if statement that was waiting for robot_moving to be implemented
//   if (gyro_get_degrees() < desired_angle) {
//     printf("%f is less than %f\n", gyro_get_degrees(), desired_angle);
//   }
//   else {
//     printf("%f is greater than %f\n", gyro_get_degrees(), desired_angle);
//   }
//
//   if (degrees > 0) {
//     while (gyro_get_degrees() < desired_angle) {
//       set_wheel_pows(-0.58, 0.58);
//       printf("current angle = %f \n", gyro_get_degrees());
//     }
//   }
//   else {
//     while (gyro_get_degrees() > desired_angle) {
//       set_wheel_pows(0.58, -0.58);
//       printf("current angle = %f \n", gyro_get_degrees());
//     }
//   }
//
//   printf("Done rotating\n");
//   wheels_brake();
//   printf("End rotate.\n\n");
// }

void rotate_by_gyro(float dtheta) {
  const float drive_max = 0.7;
  const float drive_min = 0.2;
  const float angle_tolerance = 9;
  const float correct_hold_time = 400;
  float final = gyro_get_degrees() + dtheta;

  bool correct_timer_active = false;
  uint32_t correct_timer_start = 0;

  while(!correct_timer_active || get_time() < correct_timer_start + correct_hold_time) {
    float angdiff = ang_diff(final, gyro_get_degrees());
    int direction = angdiff > 0 ? 1 : -1;
    float pow = fclamp(fabs(angdiff) / 90, drive_min, drive_max);
    set_wheel_pows(
      -direction * pow ,
      direction  * pow  );

    if (fabs(ang_diff(final, gyro_get_degrees())) < angle_tolerance) {
      if (!correct_timer_active) {
        correct_timer_start = get_time();
      }
      correct_timer_active = true;
    } else {
      correct_timer_active = false;
    }

    printf("rotating by %.2f  ", dtheta);
    printf("angdiff %.2f  ", angdiff);
    printf("gyro %.2f %> %.2f  ", gyro_get_degrees(), fmod(gyro_get_degrees(), 360));
    printf("wpows [ %.2f , %.2f ]  ", get_wheel_pows().l, get_wheel_pows().r);
    printf("timer:%i start:%i since:%i\n", correct_timer_active, correct_timer_start, get_time() - correct_timer_start);

    pause(2);
  }
}

void rotate_by_gyro_to(float theta) {
  printf("rotate_by_gyro_to(%.5f)\n", theta);
  const float angle_tolerance = 10;
  for (int i = 0; i < 3; i++) {
    rotate_by_gyro(ang_diff(theta, gyro_get_degrees()));
    gyro_set_degrees(get_vps_theta());
    printf("rotate_by_gyro_to(%.5f) iteration %i -> gyro %.5f -> vps %.5f\n", theta, i, gyro_get_degrees(), get_vps_theta());
    if (fabs(ang_diff(get_vps_theta(), theta)) < angle_tolerance) {
      printf("rotate_by_gyro_to(%.5f) done -> gyro %.5f -> vps %.5f\n", theta, gyro_get_degrees(), get_vps_theta());
      return;
    }
  }
  printf("rotate_by_gyro_to(%.5f) done -> gyro %.5f -> vps %.5f\n", theta, gyro_get_degrees(), get_vps_theta());
}

// void move_to(Point p, float velocity) {
//  //moves in a straight line to the desired point at the desired velocity
//  float dist_x = p.x - unimplemented_get_current_location().x;
//  float dist_y = p.y - unimplemented_get_current_location().y;
//  float dist = sqrt(pow(dist_x, 2)+pow(dist_y, 2));
//  float desired_angle = atan(dist_y/dist_x);

//  rotate(desired_angle - gyro_get_degrees());
//  set_wheel_pows(velocity, velocity);

// //   rotate(desired_angle-current_angle);
// //   set_wheel_pows(velocity, velocity);
//  while (dist > TARGET_TOLERANCE) {
//    // update_location();
//    dist_x = p.x - unimplemented_get_current_location().x;
//    dist_y = p.y - unimplemented_get_current_location().y;
//    dist = sqrt(pow(dist_x, 2)+pow(dist_y, 2));
//    pause(100);
//  }
//  wheels_brake();
// }

void move_for_time(float velocity, uint32_t millis) {
  printf("Moving at vel [%f] for time: %ims", velocity, millis);
  set_wheel_pows(velocity, velocity);
  pause(millis);
  wheels_brake();
}

// ~1.2mm overshoot
void move_distance_by_encoders(float distance_mm) {
  const float drive_max = 0.45;
  const float drive_min = 0.25;
  const float catchup_max = 50; // distance by which the wheels are allowed to skew

  set_wheel_pows(0, 0);

  int direction = distance_mm > 0 ? 1 : -1;
  if (direction == -1) distance_mm = fabs(distance_mm);

  l_r_uint16_t encoders_start = get_encoders();
  l_r_float_t delta_encs_mm = {0, 0};
  while (delta_encs_mm.l < distance_mm || delta_encs_mm.r < distance_mm) {
    delta_encs_mm.l = (get_encoders().l - encoders_start.l) * MM_PER_TICK_WHEELS;
    delta_encs_mm.r = (get_encoders().r - encoders_start.r) * MM_PER_TICK_WHEELS;

    float wheel_pow_maybe_l = direction * fclamp( (distance_mm - delta_encs_mm.l) / 400 * (drive_max - drive_min) + drive_min, 0, 1 );
    float wheel_pow_maybe_r = direction * fclamp( (distance_mm - delta_encs_mm.r) / 400 * (drive_max - drive_min) + drive_min, 0, 1 );

    bool allow_wheel_l = delta_encs_mm.l < delta_encs_mm.r + catchup_max;
    bool allow_wheel_r = delta_encs_mm.r < delta_encs_mm.l + catchup_max;

    set_wheel_pows(
      (delta_encs_mm.l < distance_mm && allow_wheel_l) ? wheel_pow_maybe_l : 0 ,
      (delta_encs_mm.r < distance_mm && allow_wheel_r) ? wheel_pow_maybe_r : 0 );

    printf("traveled: [%f, %f]  ", delta_encs_mm.l, delta_encs_mm.r);
    printf("remaining: [%f, %f]  ", distance_mm - delta_encs_mm.l, distance_mm - delta_encs_mm.r);
    printf("wheel_pows [%f, %f]\n", get_wheel_pows().l, get_wheel_pows().r);

    pause(2);
  }

  set_wheel_pows(0, 0);

  printf("move_distance_by_encoders stats: traveled [%f][%f]  target [%f][%f]  diff[%f][%f]\n",
    delta_encs_mm.l, delta_encs_mm.r,
    distance_mm, distance_mm,
    delta_encs_mm.l - distance_mm,delta_encs_mm.r - distance_mm);
}

// ~1.2mm overshoot
// TODO: handle backwards
void move_distance_by_encoders_gyro(float distance_mm) {
  const float drive_max = 0.45;
  const float drive_min = 0.3;

  set_wheel_pows(0, 0);

  float starting_angle = gyro_get_degrees();
  // int direction = distance_mm > 0 ? 1 : -1;
  // if (direction == -1) distance_mm = fabs(distance_mm);

  l_r_uint16_t encoders_start = get_encoders();
  l_r_float_t delta_encs_mm = {0, 0};
  float avg_dist_traveled = 0;
  while (avg_dist_traveled < distance_mm) {
    delta_encs_mm.l = (get_encoders().l - encoders_start.l) * MM_PER_TICK_WHEELS;
    delta_encs_mm.r = (get_encoders().r - encoders_start.r) * MM_PER_TICK_WHEELS;

    float avg_dist_traveled = delta_encs_mm.l + delta_encs_mm.r / 2;
    float angdiff = ang_diff(starting_angle, gyro_get_degrees());

    float pow_com = fclamp( (distance_mm - avg_dist_traveled) / 400 * (drive_max - drive_min) + drive_min, 0, 1 );
    float pow_bias = angdiff / 90;

    set_wheel_pows(
      pow_com - pow_bias ,
      pow_com + pow_bias );

    printf("traveled: [%.2f, %.2f] <%.2f>  ", delta_encs_mm.l, delta_encs_mm.r, avg_dist_traveled);
    printf("remaining: [%.2f, %.2f] <%.2f>  ", distance_mm - delta_encs_mm.l, distance_mm - delta_encs_mm.r, distance_mm - avg_dist_traveled);
    printf("gyro_diff: %.2f  ", angdiff);
    printf("pows: %.2f %.2f  ", pow_com, pow_bias);
    printf("wheel_pows [%.2f, %.2f]\n", get_wheel_pows().l, get_wheel_pows().r);

    pause(2);
  }

  set_wheel_pows(0, 0);

  printf("move_distance_by_gyro_encoders stats: traveled [%f][%f]  target [%f][%f]  diff[%f][%f]  angle_bias %f\n",
    delta_encs_mm.l, delta_encs_mm.r,
    distance_mm, distance_mm,
    delta_encs_mm.l - distance_mm,delta_encs_mm.r - distance_mm,
    ang_diff(starting_angle, gyro_get_degrees()));
}


void rotate_by_encoders(float delta_theta) {
  const int direction = delta_theta > 0 ? 1 : -1;
  l_r_uint16_t encoders_start = get_encoders();
  l_r_float_t delta_encs_mm = {0, 0};
  const float target_circumferential_distance = fabs(delta_theta) / 90 * M_PI * MM_WHEEL_FROM_CENTER;

  set_wheel_pows(-direction * 0.3, direction * 0.3);
  while(delta_encs_mm.l + delta_encs_mm.r < target_circumferential_distance) {
    delta_encs_mm.l = (get_encoders().l - encoders_start.l) * MM_PER_TICK_WHEELS;
    delta_encs_mm.r = (get_encoders().r - encoders_start.r) * MM_PER_TICK_WHEELS;
    printf("delta_encs_mm: [%f, %f +> %f]  ==> [%f]\n",
      delta_encs_mm.l, delta_encs_mm.r, delta_encs_mm.l + delta_encs_mm.r, target_circumferential_distance);
  }

  printf("stopping...");
  set_wheel_pows(0, 0);
  printf(" done\n");  
}
