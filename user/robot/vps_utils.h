struct Point vps_coords(uint8_t target) {
	copy_objects();
	Point ret;
	float board_to_mm = 0.5953;
	ret.x = game.coords[target].x * board_to_mm;
	ret.y = game.coords[target].y * board_to_mm;
	return ret;
}

float vps_theta() {
	copy_objects();
	float board_to_degrees = 0.08789;
	return game.coords[0].theta * board_to_degrees;
}
