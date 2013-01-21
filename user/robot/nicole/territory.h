float terr_angles[7] = {-120,-60,0,60,120,180,240};

uint8_t current_territory() {
	return territory_of_point(get_vps_current());
}

uint8_t territory_of_point(Point p) {
	float ang = atan2(p.y, p.x) / DEGS_TO_RADS;
	for(int i=0;i<6;i++){
		if((ang>terr_angles[i] && ang<terr_angles[i+1])
		|| (ang+360>terr_angles[i] && ang+360<terr_angles[i+1]))  {
			return i;
		}	
	}
	return 255;
}