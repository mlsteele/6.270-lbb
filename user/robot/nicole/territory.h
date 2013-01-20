float terr_angles[6] = {30,90,150,210,270,330};

uint8_t current_territory() {
	return territory_of_point(get_vps_current());
}

uint8_t territory_of_point(Point p) {
	float ang = atan2(p.y, p.x) * RADS_TO_DEGREES;
	int i = 0;
	while (ang<terr_angles[i]) {
		i++;
	}
	return i;	
}