#include "SkinChanger.h"
#include "Memory.h"
#include <Windows.h>
void SkinChanger::doSkinChanger()
{
	static int old[4] = { 0 };
	uintptr_t clientState = memory::get().read<uintptr_t>(GlobalVars::get().engine + GlobalVars::get().ofs.dwClientState);
	for (int i = 0; i < 4; i++) {
		DWORD nigga = memory::get().read<DWORD>(GlobalVars::get().client + GlobalVars::get().ofs.dwEntityList + ((memory::get().read<int>(GlobalVars::get().localPlayer->getBase() + GlobalVars::get().ofs.m_hMyWeapons + i * 0x4) & 0xFFF) - 1) * 0x10);
		short wpIndex = memory::get().read<short>(nigga + GlobalVars::get().ofs.m_iItemDefinitionIndex);
		if (old[i] != wpIndex)
		{
			memory::get().write(clientState + GlobalVars::get().ofs.clientstate_delta_ticks, -1);
		}
		old[i] = wpIndex;
		if (wpIndex > 0 && wpIndex < 5035)
		{
			std::string customName;
			customName.resize(32);
			customName = getCustomNameById(wpIndex);
			memory::get().write<short>(nigga + GlobalVars::get().ofs.m_iItemIDHigh, -1);
			memory::get().write<int>(nigga + GlobalVars::get().ofs.m_nFallbackPaintKit, getSkinById(wpIndex));//fallbackpaintkit
			memory::get().write<float>(nigga + GlobalVars::get().ofs.m_nFallbackSeed, 0.1f);//seed
			memory::get().write<int>(nigga + GlobalVars::get().ofs.m_nFallbackStatTrak, 1337);//stattrack
			memory::get().write<float>(nigga + GlobalVars::get().ofs.m_flFallbackWear, getWearById(wpIndex));//wear
			memory::get().write(nigga + GlobalVars::get().ofs.m_szCustomName, customName);//wear
		}
	}
}



int SkinChanger::getStatTrackById(int id)
{
	int arrKey = LocalPlayer::get().getWeaponNameInArray(id);
	return Config::get().cfgWeapons[arrKey].statTrack;
}

int SkinChanger::getSkinById(int id)
{
	int arrKey = LocalPlayer::get().getWeaponNameInArray(id);
	return Config::get().cfgWeapons[arrKey].skinId;
}

int SkinChanger::getIdOfSkinByComboList(int id)
{
	switch (id)
	{
	case 0:
		return cherepa;
	case 1:
		return bloody_web;
	case 2:
		return fade;
	case 3:
		return blaze;
	case 4:
		return fireSerpent;
	case 5:
		return DLor;
	case 6:
		return Aquamarine;
	case 7:
		return HotRod;
	case 8:
		return Medusa;
	case 9:
		return Metallic_DDPAT;
	case 10:
		return NeoNoir;
	case 11:
		return BOOM;
	case 12:
		return Emerald;
	case 13:
		return Asiimov;
	default:
		break;
	}
}

float SkinChanger::getWearById(int id)
{
	int arrKey = LocalPlayer::get().getWeaponNameInArray(id);
	return Config::get().cfgWeapons[arrKey].wear;
}

std::string SkinChanger::getCustomNameById(int id)
{
	int arrKey = LocalPlayer::get().getWeaponNameInArray(id);
	return Config::get().cfgWeapons[arrKey].customName;
}
