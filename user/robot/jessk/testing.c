#include <stdio.h>
#include <math.h>
#define DEGS_TO_RADS 0.01745328f

typedef struct {
	float x;
	float y;
} Point;

float terr_angles[7] = {-120,-60,0,60,120,180,240};

uint8_t territory_of_point(Point p) {
	float ang = atan2(p.y, p.x) / DEGS_TO_RADS;
	printf("\tAngle: %.2f\n", ang);
	int i;
	for(i=0;i<6;i++){
		if((ang>terr_angles[i] && ang<terr_angles[i+1])
		|| ((ang+360)>terr_angles[i] && (ang+360)<terr_angles[i+1]))  {
			return i;
		}	
	}
	return -1;
}

int main () {

	Point territories[6];
	territories[0] = (Point) {0,    -858};
	territories[1] = (Point) {743,  -429};
	territories[2] = (Point) {743,   429};
	territories[3] = (Point) {0,     858};
	territories[4] = (Point) {-743,  429};
	territories[5] = (Point) {-743, -429};

	int i;
	for(i=0; i<6; i++){
		printf("territory_of_point(%.0f, %.0f) = %d\n", territories[i].x, territories[i].y, territory_of_point(territories[i]));
	}
    return 0;
}
