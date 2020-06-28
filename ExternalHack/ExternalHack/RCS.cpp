#include "RCS.h"
#include "AimBot.h"
void RCS::doRCS()
{
	static vec3 oldAimPunchAngel = { 0,0,0 };
	static uintptr_t dwClientState = memory::get().read<uintptr_t>(GlobalVars::get().engine + GlobalVars::get().ofs.dwClientState);
	vec3 aimPunchAngle;
	if (GlobalVars::get().localPlayer->GetShotsFired() > 1)
	{		 		
		aimPunchAngle = memory::get().read<vec3>(GlobalVars::get().localPlayer->getBase() + GlobalVars::get().ofs.m_aimPunchAngle);
		aimPunchAngle.x *= 2.05f;
		aimPunchAngle.y *= 2.05f;
		vec3 viewAngle = memory::get().read<vec3>(dwClientState + GlobalVars::get().ofs.dwClientState_ViewAngles);
		viewAngle.x = viewAngle.x - (aimPunchAngle.x - oldAimPunchAngel.x);
		viewAngle.y = viewAngle.y - (aimPunchAngle.y - oldAimPunchAngel.y);
		oldAimPunchAngel = aimPunchAngle;
		AimBot::get().clamp(viewAngle.x, viewAngle.y);
		memory::get().write<float>(dwClientState + GlobalVars::get().ofs.dwClientState_ViewAngles,viewAngle.x);
		memory::get().write<float>(dwClientState + GlobalVars::get().ofs.dwClientState_ViewAngles + 0x4, viewAngle.y);
	}
	else {
		oldAimPunchAngel = { 0,0,0 };
	}
}


