#include "vec3.h"
#include <math.h>
float vec3::length()
{
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

float vec3::distance(vec3 f, vec3 f1)
{
	return sqrt(pow(f1.x - f.x, 2) + pow(f1.y - f.y, 2) + pow(f1.z - f.z, 2));
}
