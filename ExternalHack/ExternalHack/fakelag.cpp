#include "fakelag.h"

void fakelag::doFakeLag()
{	
	 clientState = memory::get().read<uintptr_t>(GlobalVars::get().engine + GlobalVars::get().ofs.dwClientState);
	 choked_packets = memory::get().read<int>(clientState + GlobalVars::get().ofs.clientstate_choked_commands);
	if (choked_packets >= Config::get().factor)
	{
		memory::get().write<bool>(GlobalVars::get().engine + GlobalVars::get().ofs.dwbSendPackets, true);
	}
	else  {
		memory::get().write<bool>(GlobalVars::get().engine + GlobalVars::get().ofs.dwbSendPackets, false);
	}
}

void fakelag::shutdown()
{
	memory::get().write<bool>(GlobalVars::get().engine + GlobalVars::get().ofs.dwbSendPackets, true);
}
