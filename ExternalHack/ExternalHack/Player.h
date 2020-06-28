#pragma once
#include "LocalPlayer.h"
#include <Windows.h>

class Player : public LocalPlayer
{
public:
	Player() {};
	Player(uintptr_t addr) { base = addr; };
	vec3 origin2D;
	vec3 head2D;
	vec3 head2DByOrigin;
	vec3 head3DByOrigin;
	vec3 headPos3D;
	vec3 headPos2D;
	float height2D;
	float width2D;
	float distance;
	bool bestAimTarget = false;
	bool onFov = false;
	uintptr_t iGlow;
	uintptr_t getIGlow();
private:
	
};

