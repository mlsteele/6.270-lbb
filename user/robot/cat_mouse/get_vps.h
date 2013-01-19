#ifndef _VPS_H_
#define _VPS_H_

#include <hw_config.h>
#include <Point.h>

Point get_vps_target() {
	copy_objects();
	Point ret; 
	ret.x = game.coords[1].x * UNITS_VPS_TO_MM;
	ret.y = game.coords[1].y * UNITS_VPS_TO_MM;
	return ret;
}
Point get_vps_current() {
	copy_objects();
	Point ret; 
	ret.x = game.coords[0].x * UNITS_VPS_TO_MM;
	ret.y = game.coords[0].y * UNITS_VPS_TO_MM;
	return ret;
}
float get_vps_theta() {
	copy_objects();
	return game.coords[0].theta * UNITS_VPS_TO_DEG;
}
void vps_dump() {
	Point target = get_vps_target();
	Point current = get_vps_current();
	printf("Target: (%.2f, %.2f)\n", target.x, target.y);
	printf("Current: (%.2f, %.2f)\n", current.x, current.y);
	printf("Theta: %.2f\n", get_vps_theta());
}

#endif
