#include "drv_math.h"

float normalizeAngle(float angle) 
{
	float a = fmod(angle, 2 * PI);  //取余保证角度是-360~360度
	if (a >= 0) {
		return a;
	} else {
		return a + 2 * PI;
	}
}