#pragma once
#include "Singleton.h"
#include <Windows.h>
#include "Player.h"
#include "Memory.h"
class Glow : public Singleton <Glow>
{
public:
	void doGlow(Player* player);
	void getGlowObject();
private:
	uintptr_t GlowObjectManager;
};

