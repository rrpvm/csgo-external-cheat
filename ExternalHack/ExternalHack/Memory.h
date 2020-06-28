#pragma once
#include "Singleton.h"
#include <Windows.h>
#include "vec3.h"
#include <TlHelp32.h>
#include "GlobalVars.h"
class memory : public Singleton <memory>
{
public:		
	DWORD getProcessId(const char* name);
	uintptr_t GetBase(const char* name);
	template <typename T>
	void write(uintptr_t addres, T buffer);
	template <typename T>
	T read(uintptr_t addres);
};
template<typename T>
inline void memory::write(uintptr_t addres, T buffer)
{
	WriteProcessMemory(GlobalVars::get().csgo, LPVOID(addres), &buffer, sizeof(buffer), 0);
}
template<typename T>
inline T memory::read(uintptr_t addres)
{
	T buffer;
	ReadProcessMemory(GlobalVars::get().csgo, LPVOID(addres), &buffer, sizeof(buffer), 0);
	return buffer;
}
