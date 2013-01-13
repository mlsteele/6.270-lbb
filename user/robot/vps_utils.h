struct Point vps_coords() {
	copy_objects();
	ret = struct Point;
	float board_to_mm = 0.5953;
	ret.x = game.coords[0].x * board_to_mm;
	ret.y = game.coords[0].y * board_to_mm;
	return ret;
}
float vps_theta() {
	copy_objects();
	float board_to_degrees = 0.08789;
	return game.coords[0].theta * board_to_degrees;
}