#pragma once
#include "Singleton.h"
#include <Windows.h>
#include "Memory.h"
class fakelag : public Singleton <fakelag>
{
public:
	void doFakeLag();
	void shutdown();
	int getChockedPackets() { return choked_packets; }
private:
	uintptr_t clientState = 0;
	int choked_packets = 0;
};

