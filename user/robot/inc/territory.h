float terr_angles[7];
Point territories[] = { (Point){0,    -858}, (Point){743,  -429}, (Point){743,   429}, 
	(Point){0,     858}, (Point){-743,  429}, (Point){-743, -429} };
Point gears[] = { (Point){297.2, -1206.4}, (Point){1193.4, -346}, (Point){896.3, 860.5},
	(Point){-297.2, 1206.4} (Point){-1193.4, 346}, (Point){-896.3, -860.5} };
Point mines[] = { (Point){-291, -1215}, (Point){907, -859}, (Point){1197, 356}, (Point){291, 1215},
	(Point){-907, 859}, (Point){-1197, -356} }; 

uint8_t territory_of_point(Point p);
uint8_t current_territory();

void move_to_territory(uint8_t territory);
void move_to_next_territory();