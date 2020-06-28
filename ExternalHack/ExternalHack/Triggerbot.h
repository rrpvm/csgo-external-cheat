#pragma once
#include "Singleton.h"
#include "GlobalVars.h"
class Triggerbot : public Singleton <Triggerbot>
{
public:
	void doTriggerbot();
	bool canShoot();
private:
};

