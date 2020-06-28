#include "Player.h"
#include "Memory.h"

uintptr_t Player::getIGlow()
{
	iGlow =  memory::get().read<uintptr_t>(getBase() + GlobalVars::get().ofs.m_iGlowIndex);
	return iGlow;
}
