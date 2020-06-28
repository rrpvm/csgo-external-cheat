#include "vec2.h"
#include <math.h>
vec2::vec2(int a, int b)
{
	this->x = a; this->y = b;
}

vec2::vec2(float a, float b)
{
	this->x = a; this->y = b;
}

vec2 vec2::operator+(vec2 a)
{
	return vec2(this->x + a.x, this->y + a.y);
}

vec2 vec2::operator-(vec2 a)
{
	return vec2(this->x - a.x, this->y - a.y);
}

vec2 vec2::operator*(vec2 a)
{
	return vec2(this->x * a.x, this->y * a.y);
}

vec2 vec2::operator/(vec2 a)
{
	return vec2(this->x / a.x, this->y / a.y);
}

vec2 vec2::operator+=(vec2 a)
{
	return vec2(this->x + a.x, this->y + a.y);
}

vec2 vec2::operator-=(vec2 a)
{
	return vec2(this->x - a.x, this->y - a.y);
}

vec2 vec2::operator-=(int x)
{
	return vec2(this->x - x, this->y - x);
}

vec2 vec2::operator+=(int x)
{
	return vec2(this->x + x, this->y + x);
}

vec2 vec2::operator+(int x)
{
	return vec2(this->x + x, this->y + x);
}

vec2 vec2::operator-(int x)
{
	return vec2(this->x - x, this->y - x);
}

vec2 vec2::operator/(float a)
{
	return vec2(this->x / a, this->y / a);
}

float vec2::length()
{
	return(sqrt(pow(this->x, 2.f) + pow(this->y, 2.f)));
}


