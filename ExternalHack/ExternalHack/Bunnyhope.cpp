#include "Bunnyhope.h"
#include <thread>
#include <chrono>
enum FLAGS {
	FL_ON_GROUND = 257,
	FL_JUMP = 256,
	FL_DUCK = 263,
};
void Bunnyhope::WriteBHOP()
{
	if (GetAsyncKeyState(VK_SPACE))
	{
		if (memory::get().read<int>(GlobalVars::get().localPlayer->getBase() + GlobalVars::get().ofs.dwFlags) == FL_ON_GROUND)
		{
			memory::get().write<int>(GlobalVars::get().client + GlobalVars::get().ofs.dwForceJump, 5);//press 
			static DWORD now = GetTickCount64();
			std::this_thread::sleep_for(std::chrono::milliseconds(12));
				memory::get().write<int>(GlobalVars::get().client + GlobalVars::get().ofs.dwForceJump, 4);//press 
			
		}
	}
}
