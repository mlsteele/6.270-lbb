#include <joyos.h>
#include <Point.h>

Point fake_active_target;
struct lock fake_data_lock;

bool fake_daemon_has_run = false;

Point generate_new_target() {
  Point target = {(rand()%4096-2048)*UNITS_VPS_TO_MM, (rand()%4096-2048)*UNITS_VPS_TO_MM};
  return target;
}

void first_run() {
  acquire(&fake_data_lock);
    fake_active_target = generate_new_target();
  release(&fake_data_lock);
}

float dist(Point a, Point b) {
  return sqrt(pow(a.x-b.x, 2) + pow(a.y-b.y, 2));
}

void generate_data() {
  acquire(&vps_data_lock);
  acquire(&fake_data_lock);
  if (dist(vps_position, fake_active_target) < TARGET_TOLERANCE) {
    fake_active_target = generate_new_target();
  }  

  fake_daemon_has_run = true;

  release(&fake_data_lock);
  release(&vps_data_lock);
}

int fake_data_daemon() {
  srand(8);
  first_run();
  while(1) {
    pause(20); // safety interval (arbitrary time)
    generate_data();
  }

  return 0;
}

void fake_data_daemon_init() {
  init_lock(&fake_data_lock, "fake_data_lock");
  create_thread(fake_data_daemon, STACK_DEFAULT, 0, "fake_daemon");
}
