#include "Glow.h"



void Glow::doGlow(Player * player)
{

	player->getIGlow();
	memory::get().write<float>((((player->iGlow * 0x38) + 0x4) + GlowObjectManager), Config::get().ColorGlow[0]);
	memory::get().write<float>((((player->iGlow * 0x38) + 0x8) + GlowObjectManager), Config::get().ColorGlow[1]);
	memory::get().write<float>((((player->iGlow * 0x38) + 0xC) + GlowObjectManager), Config::get().ColorGlow[2]);
	memory::get().write<float>((((player->iGlow * 0x38) + 0x10) + GlowObjectManager),Config::get().chams ?  .39f : Config::get().glowThikness / 10.f + 0.1f);
	memory::get().write<bool>((((player->iGlow * 0x38) + 0x24) + GlowObjectManager), true);
	memory::get().write<bool>((((player->iGlow * 0x38) + 0x25) + GlowObjectManager), false);
	memory::get().write<bool>((((player->iGlow * 0x38) + 0x26) + GlowObjectManager), Config::get().chams);//full
	memory::get().write<uint32_t>((((player->iGlow * 0x38) + 0x002C) + GlowObjectManager), Config::get().currGlowType);// 1 = best , 2 = small thickness
}

void Glow::getGlowObject()
{
	GlowObjectManager = memory::get().read<uintptr_t>(GlobalVars::get().client + GlobalVars::get().ofs.dwGlowObjectManager);
}
