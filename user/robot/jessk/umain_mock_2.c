#include <joyos.h>
#include <hw_config.h>
#include <Point.h>
#include <moving.h>
#include <math.h>
#include <vps_data_daemon.h>
#include <territory.h>

int usetup (void) {
  extern volatile uint8_t robot_id;
  robot_id = 8;

  territory_init();
  gyro_init(PIN_GYRO, LSB_US_PER_DEG, 500);
  return 0;
}

int umain (void) {
  vps_data_daemon_init();
  bind_gyro_to_vps();

  // move around the board 
  for(int i=0;i<6;i++){
    move_to_next_territory();
    //it might not move right without pausing here
    //set_wheel_pows(0, 0);
  }

  if(owner(current_territory())!=8){
    move_to_gear();
    spin_gear();
  }


  return 0;
}
