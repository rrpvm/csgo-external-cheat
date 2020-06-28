#include "Triggerbot.h"
#include "Glow.h"
#include <thread>
#include <chrono>
void Triggerbot::doTriggerbot()
{
	if (canShoot())
	{
		if (!Config::get().safeTrigger) {
			static DWORD time = GetTickCount();
			if (GetTickCount() - time >= Config::get().delayBetweenShots) {
				time = GetTickCount();
				memory::get().write<int>(GlobalVars::get().client + GlobalVars::get().ofs.dwForceAttack, 5);
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				memory::get().write<int>(GlobalVars::get().client + GlobalVars::get().ofs.dwForceAttack, 4);
			}
		}
		else {
			static DWORD time = GetTickCount();
			if (GetTickCount() - time >= Config::get().delayBetweenShots) {
				time = GetTickCount();
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}
		}
	}
}


bool Triggerbot::canShoot()
{	
		int EntityOnCrosshair = memory::get().read<int>(GlobalVars::get().localPlayer->getBase() + GlobalVars::get().ofs.m_iCrosshairId);
		if (EntityOnCrosshair > 0 && EntityOnCrosshair < 64)
		{
			Player*  currEntity =new Player( memory::get().read<uintptr_t>(GlobalVars::get().client + GlobalVars::get().ofs.dwEntityList + (EntityOnCrosshair - 1) * 0x10));//new instead globalVars cuz has crashes
			currEntity->Update();
			if (currEntity != nullptr && currEntity != NULL) {
				if (currEntity->team != memory::get().read<int>(GlobalVars::get().localPlayer->getBase() + GlobalVars::get().ofs.m_iTeam) && currEntity->health > 0)
				{
					delete currEntity;
					return true;
				}
			}
			delete currEntity;
			return false;
		}
	return false;
}
