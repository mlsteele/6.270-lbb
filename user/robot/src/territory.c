#include <territory.h>
#include <math.h>

static territory territories[6];

territory* get_territory(int i) {
  return &territories[i];
}

int current_territory_id() {
  float angle = atan2(get_vps_position().x, get_vps_position().y) * 180 / M_PI;
  for (int i =0 ; i < 6; i++) {
    if (    fmod(angle, 360) < fmod(get_territory(i)->theta_ccw, 360)
         && fmod(angle, 360) > fmod(get_territory(i)->theta_cw , 360) ) {
        return i;
    }
  }
  return -1;
}

static uint8_t get_n_balls_i(int i) {
  acquire(&objects_lock);
  uint8_t ret = territories[i]->n_balls();
  release(&objects_lock);
  return ret;
}

static uint8_t get_owner_id_i(int i) {
  acquire(&objects_lock);
  uint8_t ret = territories[i]->get_owner_id();
  release(&objects_lock);
  return ret;
}

// forgive me, for I have sinned
static uint8_t get_n_balls_0() {return get_n_balls_i(0);}
static uint8_t get_n_balls_1() {return get_n_balls_i(1);}
static uint8_t get_n_balls_2() {return get_n_balls_i(2);}
static uint8_t get_n_balls_3() {return get_n_balls_i(3);}
static uint8_t get_n_balls_4() {return get_n_balls_i(4);}
static uint8_t get_n_balls_5() {return get_n_balls_i(5);}
static uint8_t get_owner_id_0() {return get_owner_id_i(0);}
static uint8_t get_owner_id_1() {return get_owner_id_i(1);}
static uint8_t get_owner_id_2() {return get_owner_id_i(2);}
static uint8_t get_owner_id_3() {return get_owner_id_i(3);}
static uint8_t get_owner_id_4() {return get_owner_id_i(4);}
static uint8_t get_owner_id_5() {return get_owner_id_i(5);}

static uint8_t (*n_balls_accessors[6])() = {
  get_n_balls_0,
  get_n_balls_1,
  get_n_balls_2,
  get_n_balls_3,
  get_n_balls_4,
  get_n_balls_5,
};

static uint8_t (*owner_id_accessors[6])() = {
  get_owner_id_0,
  get_owner_id_1,
  get_owner_id_2,
  get_owner_id_3,
  get_owner_id_4,
  get_owner_id_5,
};

int init_territories() {
  for (int i = 0; i < 6; i++) {
    territories[i].id = i;

    territories[i].n_balls = n_balls_accessors[i];
    territories[i].owner_id = owner_id_accessors[i];

    territories[i]._n_balls = -1;
    territories[i]._owner_id = -1;

    // territories[i].theta_cw = ??;
    // territories[i].theta_ccw = ??;

    // territories[i].center_pos = ??;
    // territories[i].dispenser = ??;
    // territories[i].capture = ??;

    // TODO: reverse?
    territories[i].cw  = &territories[(i - 1) % 6];
    territories[i].ccw = &territories[(i + 1) % 6];
  }
}
