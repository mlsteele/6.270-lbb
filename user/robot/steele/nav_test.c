#include <joyos.h>
#include "pin_config.h"
#include "navigation.h"

int usetup (void) {
  return 0;
}

int umain (void) {
  init_lock(&nav_lock, "navigation_lock");
  create_thread(navigation_controller, STACK_DEFAULT, 0, "navigation_thread");

  printf("idling...\n");
  pause(1000);

  v2d t = {20, 30};
  printf("go to <%d, %d>\n", t.x, t.y);
  set_target_pos(&t);

  pause(3000);
  printf("abort seek\n");
  set_target_pos(NULL);

  return 0;
}
