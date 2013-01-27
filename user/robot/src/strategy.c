#include <strategy.h>

#include <hw_config.h>
#include <vps_data_daemon.h>
#include <territory.h>
#include <moving.h>
#include <transport.h>

void determine_team_color() {
  printf("determining team color... ");
  team_color = get_vps_position().x > 0 ? TEAM_RED : TEAM_BLUE;
  if (team_color == TEAM_RED)
    printf("TEAM_RED\n");
  if (team_color == TEAM_BLUE)
    printf("TEAM_BLUE\n");
}
