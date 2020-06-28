#pragma once
#include <Windows.h>
#include "Singleton.h"
#include "GlobalVars.h"
#include "vec2.h"
#define PI 3.14
#define ZEROTARGET -200
class AimBot : public Singleton <AimBot>
{
public:
	vec3  GetClosestTarget();
	void DoAimWrite();
	void doAimSafe();
	vec2 calculateAimAngle(vec3 localO, vec3 boneO);
	void clamp(float& pitch, float& yaw);
private:
	vec3 OUT_ANGLES = { ZEROTARGET,ZEROTARGET,ZEROTARGET };
	uintptr_t clientState;
	uintptr_t viewAnglesAddres;
};

