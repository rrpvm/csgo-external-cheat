#pragma once
#include "Singleton.h"
class vec3 : public Singleton <vec3>
{
public:
	vec3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; };
	vec3() { };
	float x, y, z;
	float length();
	float distance(vec3 f, vec3 f1);
	vec3 operator+(vec3 a)
	{
		this->x += a.x;
		this->y += a.y;
		this->z += a.z;
		return *this;
	}
	vec3 operator-(vec3 a)
	{
		this->x -= a.x;
		this->y -= a.y;
		this->z -= a.z;
		return *this;
	}
private:
};

