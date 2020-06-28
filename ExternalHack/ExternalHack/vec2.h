#pragma once
class vec2
{
public:
	vec2() {};
	~vec2() {};
	vec2(int a, int b);
	vec2(float a, float b);
	vec2 operator+(vec2 a);
	vec2 operator-(vec2 a);
	vec2 operator*(vec2 a);
	vec2 operator/(vec2 a);
	vec2 operator+=(vec2 a);
	vec2 operator-=(vec2 a);
	vec2 operator-=(int x);
	vec2 operator+=(int x);
	vec2 operator+(int x);
	vec2 operator-(int x);
	vec2 operator/(float a);
	float length();
	float x = 0;
	float y = 0;
};

