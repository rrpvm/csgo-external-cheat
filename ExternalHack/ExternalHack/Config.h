#pragma once
#include "Singleton.h"
#include "vec3.h"
#include <Windows.h>
#include <vector>
enum AimPrefer {
	closest,
	center,
	hp,
	closest_center,
	closest_hp,
	center_hp
};


class CFG : public Singleton <CFG> {
public:
	template <typename T>
	void addVariable(T* var, const char* name, int num = 1);//22.06 updated
	void save();
	void load();
	template <typename T>
	void addToLoad(T* toChange, const char* name);
	template <typename T>
	struct variable {
		const char* name;
		T* value;
		int index;
		int arraySize;
	};
public:
	int boolCurr = 0;
	int IntCurr = 0;
	int FCurr = 0;
private:
	const char* filePath = "C:\\Users\\CFG\\config.cfg ";//incorrect 21.06 FIX THIS SHIT PLZ
	const char* fileName = "config.cfg";			
	variable <float> floatList[500];
	variable <bool> BoolsList[500];
	variable <int> IntList[500];
};

struct SkinCFG {
	std::string customName = "skin";
	const char* weaponName = "";
	int skinId =38;
	float wear = 0.01f;
	int seed = 0;
	int statTrack = 0;
	int currComboSelect = 0;
};
class Config : public Singleton<Config>
{
public:
	/*VISUALS BOOL*/
	bool BoxEsp = false;
	bool LineEsp = false;
	bool HpEsp = false;
	bool NameEsp = false;
	bool Glow = false;
	bool TeamEsp = false;
	bool isVisible = false;
	bool onVisible = false;
	bool chams = false;
	bool RenderbestAimTarget = false;
	bool showHeadPos = false;
	bool drawFOV = false;
	bool FOV_Outline = false;
	bool FOV_Indicate = false;
	bool fakelag_indicator = false;
	bool enemyWeapon = false;
	bool recoilCrz = false;
	bool weaponIcons = true;
	/*VISUALS BOOL : END*/
	/*----- <VISUALS combo & Pickers> -----*/
	float ColorGlow[4] = { 1.f,0.f,1.f,1.f };
	float ColorBox[4] = {.0f,1.f,0.f,1.f};
	float onVisibleColorBox[4] = {1.f,0.f,0.f,1.f};
	float fovColor[4] = { 1.f,0.f,1.f,1.f };
	const char* boxType[3] = { "corner" , "default", "second-corner" };
	const char* hptype[7] = { "top" , "bottom", "left", "right", "top txt", "bottom txt", "up scale + txt" };
	const char* glowType[3] = { "default" , "pulse", "outline" };
	int currTypeOfBox = 1;
	int currGlowType = 1;
	int hpType = 0;
	int glowThikness = 5;
	/*----- </VISUALS combo> -----*/

	/*----- <AIMBOT> -----*/
	bool WriteAimBot = false;
	bool SafeAimBot = false;
	bool TeamAim = false;
	bool aimDormantCheck = false;
	bool RCS = false;
	bool Trigger = false;
	bool TriggerSafe = false;
	bool fake_lag = false;
	/*----- </AIMBOT> -----*/

	/*----- <AIMBOT Combo> -----*/		
	DWORD aimKey = VK_MENU;
	int Smooth = 1;
	int FOV = 1;
	int delayBetweenShots = 1;
	int factor = 1;
	const char* aim_preference[4] = { "head" , "body", "pelvis", "Neck"};
	int boneList[4] = {8,0,9,70};
	int currBone = 0;
	/*----- </AIMBOT Combo> -----*/
	bool BHOP = false;
	bool safeBHOP = false;
	bool safeTrigger = false;


	bool skingChanger = false;
	const int skinItems = 22;//change
	int currComboWeaponSkin = 0;
	const char* skinchanger_skins[22] = {"glock-18","usp-s","p250","five-seven","dual-berets","deagle","r8", "ump-45", "mp-9","mp-7", "mac-10","famas","Galil-Ar","ak-47","m4a1", "m4a1-s","ssg08","awp","scar-20","G3SG1","aug", "sg553" };
	const char* skins[14] = {"4erepa","crimson web", "fade", "blaze", "fire serpent", "Dragon Lore","Aquamarine", "Hot Rod","Medusa","MetalicDDPAT", "Neo Noir","BOOM","Emerald","Asiimov" };
	SkinCFG cfgWeapons[22];

	float menuCheckboxWarningColor[4] = { .8f,.3f,0.2f,1.f };
	float menuCheckboxesColor[4] = { .1f,.1f,.1f,1.f };
	float menuFrameColor[4] = { 0.f,0.f,0.f,1.f };
	float menuCheckedCBoxes[4] = { .11f,.6f,0.05f,1.f };
	float menuChildColor[4] = { .1f,.1f,.1f,1.f };
	float menuButtonColor[4] = { .1f,.1f,.1f,1.f };

	void SafeCfg();
	void LoadCfg();
	void ResetCFG();
private:
};

template<typename T>
inline void CFG::addVariable(T * var, const char * name, int num)
{
	if (typeid(T) == typeid(bool)) {
		BoolsList[boolCurr].name = name;
		BoolsList[boolCurr].value = reinterpret_cast<bool*>(var);
		boolCurr++;
	}
	if (typeid(T) == typeid(int))
	{
		IntList[IntCurr].name = name;
		IntList[IntCurr].value = reinterpret_cast<int*>(var);
		IntCurr++;
	}
	if (typeid(T) == typeid(float))
	{
		floatList[FCurr].name = name;
		floatList[FCurr].value = reinterpret_cast<float*>(var);
		floatList[FCurr].arraySize = num;
		FCurr++;
	}
}

template<typename T>
inline void CFG::addToLoad(T * toChange, const char * name)
{
	if (typeid(T) == typeid(bool)) {
		BoolsList[boolCurr].value = (bool*)toChange;
		BoolsList[boolCurr].name = name;
		boolCurr++;
	}
	if (typeid(T) == typeid(int))
	{
		IntList[IntCurr].value = (int*)toChange;
		IntList[IntCurr].name = name;
		IntCurr++;
	}
	if (typeid(T) == typeid(float))
	{
		floatList[FCurr].value = (float*)toChange;
		floatList[FCurr].name = name;
		FCurr++;
	}
}
