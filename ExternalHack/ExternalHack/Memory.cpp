#include "Memory.h"

# if defined(_MSC_VER)
# ifndef _CRT_SECURE_NO_DEPRECATE
# define _CRT_SECURE_NO_DEPRECATE (1)
# endif
# pragma warning(disable : 4996)
# endif


DWORD memory::getProcessId(const char * name)
{
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hDump = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(hDump, &pe32))
	{
		do {
			if (!stricmp(name, pe32.szExeFile))
			{
				CloseHandle(hDump);
				return pe32.th32ProcessID;
				break;//dumb code
			}
		} while (Process32Next(hDump, &pe32));
	}
	return 0;
}
uintptr_t memory::GetBase(const char * name)
{
	MODULEENTRY32 me32;
	me32.dwSize = sizeof(MODULEENTRY32);
	HANDLE hDump = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GlobalVars::get().pId);
	if (Module32First(hDump, &me32))
	{
		do {
			if (!stricmp(name, me32.szModule))
			{
				CloseHandle(hDump);
				return (uintptr_t)me32.modBaseAddr;
				break;//dumb code
			}
		} while (Module32Next(hDump, &me32));
	}
	return -1337;
}