#include <util.h>
#include <hw_config.h>
#include <moving.h>
#define DELAY 20

static struct lock encoder_data_lock;
static bool possibly_stalled;
static uint16_t stalled_timer;
static bool robot_stuck;
static l_r_float_t enc_speed;
static l_r_uint16_t last_enc;
static l_r_uint16_t enc;

void calculate_enc_speed() {
	acquire(&encoder_data_lock);
	enc.l = encoder_read(PIN_ENCODER_WHEEL_L);
	enc.r = encoder_read(PIN_ENCODER_WHEEL_R);
	enc_speed.l = (enc.l - last_enc.l) * MM_PER_TICK_WHEELS / DELAY;
	enc_speed.r = (enc.r - last_enc.r) * MM_PER_TICK_WHEELS / DELAY;
	possibly_stalled = (get_wheel_pows().l != 0 || get_wheel_pows().r != 0) && 
		(enc_speed.l == 0 && enc_speed.r == 0)
	if (possibly_stalled) {
		stalled_timer += DELAY;
		robot_stuck = stalled_timer >= 1000
	}
	else {
		stalled_timer = 0;
		robot_stuck = false;
	}
	last_enc.l = enc_l;
	last_enc.r = enc_r;
	release(&encoder_data_lock);
}

void encoder_data_daemon() {
	while(1) {
		pause(DELAY);
		calculate_enc_speed();
	}
}

void encoder_daemon_init() {
	init_lock(&encoder_data_lock, "encoder_data_lock");
	last_enc_l = 0;
	last_enc_r = 0;
	create_thread(encoder_data_daemon, STACK_DEFAULT, 0, "encoder_daemon");
}

l_r_float_t enc_speed() {
	//mm per milli
	acquire(&encoder_data_lock);
	l_r_float_t ret = enc_speed;
	release(&encoder_data_lock);
	return ret;	
}

bool robot_stuck() {
	acquire(&encoder_data_lock);
	bool ret = robot_stuck;
	release(&encoder_data_lock);
	return ret;
}