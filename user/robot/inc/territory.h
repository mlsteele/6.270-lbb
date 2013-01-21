float terr_angles[7];
Point territories[6];

uint8_t territory_of_point(Point p);
uint8_t current_territory();

void move_to_territory(uint8_t territory);
void move_to_next_territory();