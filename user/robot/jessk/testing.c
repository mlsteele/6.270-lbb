#include <stdio.h>
#include <math.h>
#define DEGS_TO_RADS 0.01745328f

typedef struct {
	float x;
	float y;
} Point;
float ang_diff(float a, float b) {
  float delta_theta = fmod(a, 360) - fmod(b, 360);
  if (delta_theta < -180) delta_theta += 360;
  if (delta_theta >  180) delta_theta -= 360;
  return delta_theta;
}

int8_t territory_of_point(Point p) {
	float ang = fmod(ang_diff(atan2(p.y, p.x) / DEGS_TO_RADS,0) + 360,360);
	printf("ang = %.0f\n", ang);
	int i;
	for(i=0;i<6;i++){
		printf("\tang_diff(ang, %.0f): %.0f\n", fmod(270+60*i,360), ang_diff(ang, fmod(270+60*i,360)));
		if(fabsf(ang_diff(ang, fmod(270+60*i,360))) <= 60) {
	    	//printf("  YES!\n");
			return i;
		}	//printf("  NO!\n");
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
