#ifndef _MOVING_H_
#define _MOVING_H_

#include <math.h>
#include <hw_config.h>
#include <Point.h>
// #include <hybrid_integrator_daemon.h>
// #include <encoder_integrator_daemon.h>

l_r_float_t last_motor_pows = {0, 0};

#define DEFAULT_SPEED .58
#define TARGET_TOLERANCE 50

float get_gyro_current_angle() {
	//return fmod(gyro_get_degrees(), 360);
	return gyro_get_degrees();
}

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
	motor_set_vel(PIN_MOTOR_DRIVE_L, l > -255 ? (l < 255 ? (l * 255) : 255) : -255);
	motor_set_vel(PIN_MOTOR_DRIVE_R, r > -255 ? (r < 255 ? (r * 255) : 255) : -255);

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

void rotate(float degrees) {
	printf("Rotating...\n");
	printf("start angle = %f \n", get_gyro_current_angle());
	float desired_angle = get_gyro_current_angle() + degrees;
	printf("desired_angle = %f \n", desired_angle);

	// NOTE: (miles) removed the no-op if statement that was waiting for robot_moving to be implemented
	if (get_gyro_current_angle() < desired_angle) {
		printf("%f is less than %f\n", get_gyro_current_angle(), desired_angle);
	}
	else {
		printf("%f is greater than %f\n", get_gyro_current_angle(), desired_angle);
	}

	if (degrees > 0) {
		while (get_gyro_current_angle() < desired_angle) {
			set_wheel_pows(-0.58, 0.58);
			printf("current angle = %f \n", get_gyro_current_angle());
		}
	}
	else {
		while (get_gyro_current_angle() > desired_angle) {
			set_wheel_pows(0.58, -0.58);
			printf("current angle = %f \n", get_gyro_current_angle());
		}
	}

	printf("Done rotating\n");
	wheels_brake();
	printf("End rotate.\n\n");
}

// void move_to(Point p, float velocity) {
// 	//moves in a straight line to the desired point at the desired velocity
// 	float dist_x = p.x - unimplemented_get_current_location().x;
// 	float dist_y = p.y - unimplemented_get_current_location().y;
// 	float dist = sqrt(pow(dist_x, 2)+pow(dist_y, 2));
// 	float desired_angle = atan(dist_y/dist_x);

// 	rotate(desired_angle - get_gyro_current_angle());
// 	set_wheel_pows(velocity, velocity);

// // 	rotate(desired_angle-current_angle);
// // 	set_wheel_pows(velocity, velocity);
// 	while (dist > TARGET_TOLERANCE) {
// 		// update_location();
// 		dist_x = p.x - unimplemented_get_current_location().x;
// 		dist_y = p.y - unimplemented_get_current_location().y;
// 		dist = sqrt(pow(dist_x, 2)+pow(dist_y, 2));
// 		pause(100);
// 	}
// 	wheels_brake();
// }

float get_heading() {
	//do some fancy averaging thing instead
	return gyro_get_degrees();
}

void move_for_time(float velocity, uint32_t millis) {
	printf("Moving at vel [%f] for time: %ims", velocity, millis);
	set_wheel_pows(velocity, velocity);
	pause(millis);
	wheels_brake();
}

// ~1.2mm overshoot
void move_distance_by_encoders(float distance_mm) {
	set_wheel_pows(0, 0);

	int direction = distance_mm > 0 ? 1 : -1;
	if (direction == -1) distance_mm = fabs(distance_mm);

	float drive_max = 0.68;
	float drive_min = 0.25;

	l_r_uint16_t encoders_start = get_encoders();
	l_r_float_t delta_encs_mm = {0, 0};
	while (delta_encs_mm.l < distance_mm || delta_encs_mm.r < distance_mm) {
		delta_encs_mm.l = (get_encoders().l - encoders_start.l) * MM_PER_TICK_WHEELS;
		delta_encs_mm.r = (get_encoders().r - encoders_start.r) * MM_PER_TICK_WHEELS;

		float wheel_pow_maybe_l = direction * fclamp( (distance_mm - delta_encs_mm.l) / 400 * (drive_max - drive_min) + drive_min, 0, 1 );
		float wheel_pow_maybe_r = direction * fclamp( (distance_mm - delta_encs_mm.r) / 400 * (drive_max - drive_min) + drive_min, 0, 1 );

		set_wheel_pows(
			(delta_encs_mm.l < distance_mm) ? wheel_pow_maybe_l : 0 ,
			(delta_encs_mm.r < distance_mm) ? wheel_pow_maybe_r : 0 );

		printf("wheel_pows [%f, %f]    traveled: [%f, %f]\n",
			get_wheel_pows().l, get_wheel_pows().r, delta_encs_mm.l - distance_mm, delta_encs_mm.r - distance_mm);

		pause(2);
	}

	set_wheel_pows(0, 0);

	printf("move_distance_by_encoders stats: traveled [%f][%f]  target [%f][%f]  diff[%f][%f]\n",
		delta_encs_mm.l, delta_encs_mm.r, distance_mm, distance_mm, delta_encs_mm.l - distance_mm, delta_encs_mm.r - distance_mm);
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


#endif
