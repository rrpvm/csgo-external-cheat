#include "LocalPlayer.h"
#include "Memory.h"
#include "GlobalVars.h"
# if defined(_MSC_VER)
# ifndef _CRT_SECURE_NO_DEPRECATE
# define _CRT_SECURE_NO_DEPRECATE (1)
# endif
# pragma warning(disable : 4996)
# endif
int LocalPlayer::getHealth()
{
	return memory::get().read<int>(base + GlobalVars::get().ofs.m_iHealth);
}

int LocalPlayer::getTeam()
{
	return memory::get().read<int>(base + GlobalVars::get().ofs.m_iTeam);
}

int LocalPlayer::getFlags()
{
	return memory::get().read<int>(base + GlobalVars::get().ofs.dwFlags);
}

vec3 LocalPlayer::getOrigin()
{
	return memory::get().read<vec3>(base + GlobalVars::get().ofs.m_vecOrigin);
}

void LocalPlayer::Update()
{
	health = getHealth();
	team = getTeam();
	flags = getFlags();
	origin = getOrigin();
	viewOffset = GetviewOffset();
	isDormant = getDormant();
	inScope = isScoping();
	spotted = memory::get().read<bool>(base + GlobalVars::get().ofs.b_Spotted);
}

vec3 LocalPlayer::GetviewOffset()
{
	return memory::get().read<vec3>(base + GlobalVars::get().ofs.vecViewOffes);
}
bool LocalPlayer::getDormant()
{
	return memory::get().read<bool>(base + GlobalVars::get().ofs.dormant);
}
bool LocalPlayer::isScoping()
{
	return memory::get().read<bool>(base + GlobalVars::get().ofs.m_bIsScoped);
}
vec3 LocalPlayer::getBonePos(int boneId, uintptr_t base)
{
	uintptr_t boneBase = memory::get().read<uintptr_t>(base + GlobalVars::get().ofs.boneMatrix);
	vec3 bonePos;

	bonePos.x = memory::get().read<float>(boneBase + 0x30 * boneId + 0x0C);//0x30 = matrix3t_x4 (0x30)
	bonePos.y = memory::get().read<float>(boneBase + 0x30 * boneId + 0x1C);//0x30 = matrix3t_x4 (0x30)
	bonePos.z = memory::get().read<float>(boneBase + 0x30 * boneId + 0x2C);//0x30 = matrix3t_x4 (0x30)
	return bonePos;
}

int LocalPlayer::GetShotsFired()
{
	shotsFired = memory::get().read<int>(base + GlobalVars::get().ofs.m_iShotsFired);
	return shotsFired;
}

std::string LocalPlayer::get_weapon_name(int weaponId, bool icons)
{

	if (!this || weaponId < 0 || weaponId > 5035)
		return "error";

	switch (weaponId)
	{
	case WEAPON_NONE:  return icons ? "" : "none"; break;
	case WEAPON_DEAGLE: return icons ? "A" : "deagle"; break;
	case WEAPON_ELITE: return icons ? "B" : "dual-berets"; break;
	case WEAPON_FIVESEVEN: return icons ? "C" : "five-seven"; break;
	case WEAPON_GLOCK: return !icons ? "glock-18" : "D"; break;
	case WEAPON_AK47: return !icons ? "ak-47" : "W"; break;
	case WEAPON_AUG: return !icons ? "aug" : "U"; break;
	case WEAPON_AWP: return !icons ? "awp" : "Z"; break;
	case WEAPON_FAMAS: return !icons ? "famas" : "R"; break;
	case WEAPON_G3SG1:  return !icons ? "G3SG1" : "X"; break;
	case WEAPON_GALILAR:   return !icons ? "Galil-Ar" : "Q";  break;
	case WEAPON_M249: return !icons ? "m249" : "g"; break;
	case WEAPON_M4A1:  return !icons ? "m4a1" : "S"; break;
	case WEAPON_MAC10:  return !icons ? "mac-10" : "H"; break;
	case WEAPON_P90:  return !icons ? "petux(p90)" : "P"; break;
	case WEAPON_MP5SD:   return !icons ? "mp5-sd" : "K"; break;
	case WEAPON_UMP45:   return !icons ? "ump-45" : "L"; break;
	case WEAPON_XM1014:  return !icons ? "xm1014" : "b"; break;
	case WEAPON_BIZON:   return !icons ? "bizon" : "M"; break;
	case WEAPON_MAG7:   return !icons ? "mag-7" : "d"; break;
	case WEAPON_NEGEV:  return !icons ? "negev" : "f"; break;
	case WEAPON_SAWEDOFF:   return !icons ? "sawed-off" : "c"; break;
	case WEAPON_TEC9:  return !icons ? "tec-9" : "H"; break;
	case WEAPON_TASER:  return !icons ? "taser" : "h"; break;
	case WEAPON_HKP2000:  return !icons ? "p2000" : "E"; break;
	case WEAPON_MP7: return !icons ? "mp7" : "K"; break;
	case WEAPON_MP9:  return !icons ? "mp9" : "O"; break;
	case WEAPON_NOVA: return !icons ? "nova" : "e"; break;
	case WEAPON_P250:  return !icons ? "p250" : "F"; break;
	case WEAPON_SCAR20:  return !icons ? "scar-20" : "Y"; break;
	case WEAPON_SG556: return !icons ? "sg553" : "V"; break;
	case WEAPON_SSG08:  return !icons ? "ssg-08" : "a"; break;
	case WEAPON_KNIFEGG:  return "knife gg"; break;
	case WEAPON_KNIFE:  return !icons ? "knife" : "0"; break;
	case WEAPON_FLASHBANG: return !icons ? "flashbang" : "i"; break;
	case WEAPON_HEGRENADE: return !icons ? "hegrenade" : "j"; break;
	case WEAPON_SMOKEGRENADE: return !icons ? "smoke" : "k"; break;
	case WEAPON_MOLOTOV:  return !icons ? "molotov" : "l"; break;
	case WEAPON_DECOY:  return !icons ? "decoy" : "m"; break;
	case WEAPON_INCGRENADE:  return "incgrenade"; break;
	case WEAPON_C4: return !icons ? "c4" : "o"; break;
	case WEAPON_HEALTHSHOT:  return "healthshot"; break;
	case WEAPON_KNIFE_T: return !icons ? "knife" : "4"; break;
	case WEAPON_M4A1_SILENCER: return !icons ? "m4a1s" : "T"; break;
	case WEAPON_USP_SILENCER:  return !icons ? "usp-s" : "G"; break;
	case WEAPON_CZ75A: return !icons ? "cz75a" : "I"; break;
	case WEAPON_REVOLVER:  return !icons ? "r8" : "J";; break;
	case WEAPON_TAGRENADE:  return "tag-grenade"; break;
	case WEAPON_FISTS:  return "fists"; break;
	default:
		return !icons ? "default" : "";
		break;
	}

}

const char * LocalPlayer::get_weapon_name2(int weaponId)
{


	if (!this || weaponId < 0 || weaponId > 5035)
		return "error";

	switch (weaponId)
	{
	case 0: return "glock-18"; break;
	case 1: return "usp-s"; break;
	case 2: return "p250"; break;
	case 3: return "five-seven"; break;
	case 4: return "dual-berets"; break;
	case 5: return "deagle"; break;
	case 6: return "r8"; break;
	case 7: return "ump-45"; break;
	case 8: return "mp-9"; break;
	case 9: return "mp-7"; break;
	case 10: return "mac-10"; break;
	case 11: return "famas"; break;
	case 12: return "Galil-Ar"; break;
	case 13: return "ak-47"; break;
	case 14: return "m4a1"; break;
	case 15: return "m4a1-s"; break;
	case 16: return "ssg-08"; break;
	case 17: return "awp"; break;
	case 18: return "scar-20"; break;
	case 19: return "G3SG1"; break;
	case 20: return "aug"; break;
	case 21: return "sg553"; break;
	}

}

int LocalPlayer::getWeaponNameInArray(int weaponId)
{
	std::string buffer = get_weapon_name(weaponId);
	for (int i = 0; i < Config::get().skinItems; i++)
	{
		if (!stricmp(buffer.c_str(), get_weapon_name2(i)))
		{
			return i;
		}
	}
	return 0;
}

int LocalPlayer::getIdByName(std::string name)
{
	const char* buff = name.c_str();
	if (!strcmp(buff, "glock-18"))return 0;
	if (!strcmp(buff, "usp-s"))return 1;
	if (!strcmp(buff, "p250"))return 2;
	if (!strcmp(buff, "five-seven"))return 3;
	if (!strcmp(buff, "dual-berets"))return 4;
	if (!strcmp(buff, "deagle"))return 5;
	if (!strcmp(buff, "r8"))return 6;
	if (!strcmp(buff, "ump-45"))return 7;
	if (!strcmp(buff, "mp-9"))return 8;
	if (!strcmp(buff, "mp-7"))return 9;
	if (!strcmp(buff, "mac-10"))return 10;
	if (!strcmp(buff, "famas"))return 11;
	if (!strcmp(buff, "Galil-Ar"))return 12;
	if (!strcmp(buff, "ak-47"))return 13;
	if (!strcmp(buff, "m4a1"))return 14;
	if (!strcmp(buff, "m4a1-s"))return 15;
	if (!strcmp(buff, "ssg-08"))return 16;
	if (!strcmp(buff, "awp"))return 17;
	if (!strcmp(buff, "scar-20"))return 18;
	if (!strcmp(buff, "G3SG1"))return 19;
	if (!strcmp(buff, "aug"))return 20;
	if (!strcmp(buff, "sg553"))return 21;
}
