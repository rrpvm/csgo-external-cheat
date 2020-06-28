#include "Config.h"
#include <fstream>
#include <shlobj.h>
#include <string>
#include <iostream>
#include <time.h>
#include "LocalPlayer.h"
# if defined(_MSC_VER)
# ifndef _CRT_SECURE_NO_DEPRECATE
# define _CRT_SECURE_NO_DEPRECATE (1)
# endif
# pragma warning(disable : 4996)
# endif
void CFG::save()
{

	std::fstream file;
	file.open("cfg.cfg", std::ios::out | std::ios::in | std::ios::trunc);
	for (int i = 0; i < boolCurr; i++)
	{
		file << "[bool]";
		file << BoolsList[i].name; file << "=";
		*BoolsList[i].value == true ? file << "1\n"  : file << "0\n";
		BoolsList[i].index = i;
	}
	for (int i = 0; i < IntCurr; i++)
	{
		file << "[int]";
		file << IntList[i].name; file << "=";
		file <<std::to_string(*IntList[i].value)<< "\n";
		IntList[i].index = i;
	}
	for (int i = 0; i < FCurr; i++)
	{
		file << "[float]";
		file << floatList[i].name; file << "={";
		for (int j = 0; j < floatList[i].arraySize; j++)
		{
			file <<std::to_string( floatList[i].value[j]) << ",";
		}
		file << "}\n";
	}
	file << "Skin settings \n";
	for (int i = 0; i < sizeof(Config::get().cfgWeapons)/sizeof(Config::get().cfgWeapons[0]); i++)
	{
		Config::get().cfgWeapons[i].weaponName = LocalPlayer::get().get_weapon_name2(i);
		file << "[weapon name]=" <<Config::get().cfgWeapons[i].weaponName << "\n";
		file << "[weapon pattern]=" << Config::get().cfgWeapons[i].skinId << "\n";
		file << "[weapon seed]=" << Config::get().cfgWeapons[i].seed << "\n";
		file << "[weapon wear]=" << Config::get().cfgWeapons[i].wear << "\n";
		file << "[weapon currCombo]=" << Config::get().cfgWeapons[i].currComboSelect << "\n";
		file << "[weapon customName]=" << Config::get().cfgWeapons[i].customName << "\n";
		file << "[weapon statTrack]=" << Config::get().cfgWeapons[i].statTrack << "\n";
	}

	time_t now = time(0);
	char* dt = ctime(&now);
	file << "\n\n [CFG SYSTEM] : CONFIG saved. " << dt << "\n <!--ExternalHack created by Roman--> \n" ;
	file << "credits : Yougame.biz - https://yougame.biz/members/66500/" << "\n";
	file << "Youtube << https://www.youtube.com/channel/UCux9Try5PKp4u5FISnT4u6w<< \n";
	file.close();
}
bool contains(std::string string,  const char*  symbols)
{
	int rightS = 0;
	for (int i = 0; i < strlen(symbols); i++)
	{
		if (string[i] == symbols[i])
		{
			rightS++;
		}
	}
	if (rightS  == strlen(symbols))
	return true;
	else return false;
}
int containsAt(std::string string, char symbol)
{
	for (int i = 0; i < string.length(); i++)
	{
		if (string.c_str()[i] == symbol)
		{
			return i;
		}
	}
	return -1000;
}
void CFG::load()
{
	std::ifstream file;
	file.open("cfg.cfg", std::ios::out);
	std::string buffer;
	bool shouldRead = true;
	boolCurr = 0;
	IntCurr = 0;
	FCurr = 0;
	while (shouldRead)
	{
		std::getline(file, buffer);
		int whereRead = containsAt(buffer, '=');
		if (contains(buffer, "[bool]")) {
			for (int i = whereRead; i < buffer.length(); i++)
			{
				if (buffer[i] == '1' || buffer[i] == '0') {
					*BoolsList[boolCurr].value = buffer[i] == '1' ? true : false;
				}
				if (i + 1 == buffer.length())	boolCurr++;
			}
		}
		if (contains(buffer, "[int]"))
		{
			std::string f = "";
			for (int i = whereRead + 1; i < buffer.length(); i++)
			{
				f += buffer[i];
			}		
			*IntList[IntCurr].value = std::stoi(f);
			IntCurr++;
		}
		if (contains(buffer, "[float]"))
		{
		
			std::string f = "";
			int currValue = 0;
			for (int i = whereRead + 2; i < buffer.length() - 1; i++)
			{
				if (buffer[i] != ',')
				{
					f += buffer[i];
					floatList[FCurr].value[currValue] = std::stof(f);
				}
				else {
					currValue++;
					f.clear();
				}
			}
			FCurr++;			
		}
		for (int i = 0; i < Config::get().skinItems; i++)
		{
			if (contains(buffer, "[weapon name]="))
			{
				int whereRead = containsAt(buffer, '=');
				std::string f = "";
				for (int j = whereRead + 1; j < buffer.length(); j++)
				{
					f += buffer[j];
				}
				int ID  = LocalPlayer::get().getIdByName(f);				
			//	if (ID <= Config::get().skinItems) {
					for (int j = 0; j < 5; j++)
					{
						std::getline(file, buffer);
						if (contains(buffer, "[weapon pattern]="))
						{
							int whereRead = containsAt(buffer, '=');
							std::string patter = "";
							for (int z = whereRead + 1; z < buffer.length(); z++)
							{
								patter += buffer[z];
							}
							Config::get().cfgWeapons[ID].skinId = std::stoi(patter);
						}
						else if (contains(buffer, "[weapon seed]=")) {
							int whereRead = containsAt(buffer, '=');
							std::string seed = "";
							for (int z = whereRead + 1; z < buffer.length(); z++)
							{
								seed += buffer[z];
							}
							Config::get().cfgWeapons[ID].seed = std::stoi(seed);
						}
						else if (contains(buffer, "[weapon wear]="))
						{
							int whereRead = containsAt(buffer, '=');
							std::string wear = "";
							for (int z = whereRead + 1; z < buffer.length(); z++)
							{
								wear += buffer[z];
							}
							Config::get().cfgWeapons[ID].wear = std::stoi(wear);
						}
						else if (contains(buffer, "[weapon customName]=")) {
							int whereRead = containsAt(buffer, '=');
							std::string customName = "";
							for (int z = whereRead + 1; z < buffer.length(); z++)
							{
								customName += buffer[z];
							}
							Config::get().cfgWeapons[ID].customName = customName;
						}
						else if (contains(buffer, "[weapon currCombo]="))
						{
							int whereRead = containsAt(buffer, '=');
							std::string combo = "";
							for (int z = whereRead + 1; z < buffer.length(); z++)
							{
								combo += buffer[z];
							}
							Config::get().cfgWeapons[ID].currComboSelect = std::stoi(combo);
						}
					}
			//	}
			}
		}

		if (buffer.empty())
		{
			shouldRead = false;
		}
	}
	std::cout << "Config loaded \n";
	file.close();
}





void Config::SafeCfg()
{
	CFG::get().boolCurr = 0;
	CFG::get().IntCurr = 0;
	CFG::get().FCurr = 0;
	/*BOOLS*/
	CFG::get().addVariable<bool>(&BoxEsp, "BoxEsp");//1
	CFG::get().addVariable<bool>(&LineEsp, "LineEsp");
	CFG::get().addVariable<bool>(&HpEsp, "HpEsp");
	CFG::get().addVariable<bool>(&Glow, "Glow");
	CFG::get().addVariable<bool>(&TeamEsp, "TeamEsp");
	CFG::get().addVariable<bool>(&isVisible, "isVisible");//5
	CFG::get().addVariable<bool>(&onVisible, "onVisible");
	CFG::get().addVariable<bool>(&chams, "chams");
	CFG::get().addVariable<bool>(&RenderbestAimTarget, "RenderbestAimTarget");
	CFG::get().addVariable<bool>(&showHeadPos, "showHeadPos");
	CFG::get().addVariable<bool>(&drawFOV, "drawFOV");//10
	CFG::get().addVariable<bool>(&FOV_Outline, "FOV_Outline");
	CFG::get().addVariable<bool>(&WriteAimBot, "WriteAimBot");
	CFG::get().addVariable<bool>(&SafeAimBot, "SafeAimBot");
	CFG::get().addVariable<bool>(&TeamAim, "TeamAim");
	CFG::get().addVariable<bool>(&aimDormantCheck, "aimDormantCheck");
	CFG::get().addVariable<bool>(&RCS, "RCS");//15
	CFG::get().addVariable<bool>(&Trigger, "Trigger");
	CFG::get().addVariable<bool>(&TriggerSafe, "TriggerSafe");
	CFG::get().addVariable<bool>(&BHOP, "BHOP");
	CFG::get().addVariable<bool>(&safeBHOP, "safeBHOP");
	CFG::get().addVariable<bool>(&safeTrigger, "safeTrigger");//20
	CFG::get().addVariable<bool>(&FOV_Indicate, "FOV_Indicate");
	CFG::get().addVariable<bool>(&fake_lag, "fakelag");
	CFG::get().addVariable<bool>(&fakelag_indicator, "fakelag_indicator");
	CFG::get().addVariable<bool>(&enemyWeapon, "enemyWeapon");
	CFG::get().addVariable<bool>(&skingChanger, "skingChanger");
	CFG::get().addVariable<bool>(&recoilCrz, "recoilCrz");
	CFG::get().addVariable<bool>(&weaponIcons, "weaponIcons");
	/**/
	/*INTS*/
	CFG::get().addVariable<int>(&FOV, "FOV");
	CFG::get().addVariable<int>(&Smooth, "Smooth");
	CFG::get().addVariable<int>(&hpType, "hpType");
	CFG::get().addVariable<int>(&currTypeOfBox, "currTypeOfBox");
	CFG::get().addVariable<int>(&currGlowType, "currGlowType");
	CFG::get().addVariable<int>(&currBone, "currBone");
	CFG::get().addVariable<int>(&delayBetweenShots, "delayBetweenShots");
	CFG::get().addVariable<int>(&factor, "factor");
	CFG::get().addVariable<int>(&glowThikness, "glowThikness");


	/*FLOAT*/
	CFG::get().addVariable<float>(ColorBox, "color_box", 4);
	CFG::get().addVariable<float>(ColorGlow, "color_glow",4);
	CFG::get().addVariable<float>(onVisibleColorBox, "color_box_visible",4);
	CFG::get().addVariable<float>(fovColor, "fovColor",4);
	CFG::get().addVariable<float>(menuCheckboxWarningColor, "menuCheckboxWarningColor",4);
	CFG::get().addVariable<float>(menuCheckboxesColor, "menuCheckboxesColor",4);
	CFG::get().addVariable<float>(menuFrameColor, "menuFrameColor",4);
	CFG::get().addVariable<float>(menuCheckedCBoxes, "menuCheckedCBoxes",4);
	CFG::get().addVariable<float>(menuChildColor, "menuChildColor",4);
	CFG::get().addVariable<float>(menuButtonColor, "menuButtonColor",4);

	CFG::get().save();
}

void Config::LoadCfg()
{
	CFG::get().boolCurr = 0;
	CFG::get().IntCurr = 0;
	CFG::get().FCurr = 0;
	/*BOOLS*/
	CFG::get().addToLoad(&BoxEsp, "BoxEsp");//1
	CFG::get().addToLoad(&LineEsp, "LineEsp");
	CFG::get().addToLoad(&HpEsp, "HpEsp");
	CFG::get().addToLoad(&Glow, "Glow");
	CFG::get().addToLoad(&TeamEsp, "TeamEsp");
	CFG::get().addToLoad(&isVisible, "isVisible");//5
	CFG::get().addToLoad(&onVisible, "onVisible");
	CFG::get().addToLoad(&chams, "chams");
	CFG::get().addToLoad(&RenderbestAimTarget, "RenderbestAimTarget");
	CFG::get().addToLoad(&showHeadPos, "showHeadPos");
	CFG::get().addToLoad(&drawFOV, "drawFOV");//10
	CFG::get().addToLoad(&FOV_Outline, "FOV_Outline");
	CFG::get().addToLoad(&WriteAimBot, "WriteAimBot");
	CFG::get().addToLoad(&SafeAimBot, "SafeAimBot");
	CFG::get().addToLoad(&TeamAim, "TeamAim");
	CFG::get().addToLoad(&aimDormantCheck, "aimDormantCheck");
	CFG::get().addToLoad(&RCS, "RCS");//15
	CFG::get().addToLoad(&Trigger, "Trigger");
	CFG::get().addToLoad(&TriggerSafe, "TriggerSafe");
	CFG::get().addToLoad(&BHOP, "BHOP");
	CFG::get().addToLoad(&safeBHOP, "safeBHOP");
	CFG::get().addToLoad(&safeTrigger, "safeTrigger");//20
	CFG::get().addToLoad(&FOV_Indicate, "FOV_Indicate");
	CFG::get().addToLoad(&fake_lag, "fakelag");
	CFG::get().addToLoad(&fakelag_indicator, "fakelag_indicator");
	CFG::get().addToLoad(&enemyWeapon, "enemyWeapon");
	CFG::get().addToLoad(&skingChanger, "skingChanger");
	CFG::get().addToLoad(&recoilCrz, "recoilCrz");
	CFG::get().addToLoad(&weaponIcons, "weaponIcons");
	/*INTS*/
	CFG::get().addToLoad(&FOV, "FOV");
	CFG::get().addToLoad(&Smooth, "Smooth");
	CFG::get().addToLoad<int>(&hpType, "hpType");
	CFG::get().addToLoad<int>(&currTypeOfBox, "currTypeOfBox");
	CFG::get().addToLoad<int>(&currGlowType, "currGlowType");
	CFG::get().addToLoad<int>(&currBone, "currBone");
	CFG::get().addToLoad<int>(&delayBetweenShots, "delayBetweenShots");
	CFG::get().addToLoad<int>(&factor, "factor");
	CFG::get().addToLoad<int>(&glowThikness, "glowThikness");

	/*FLOAT*/
	CFG::get().addToLoad<float>(ColorBox, "color_box");
	CFG::get().addToLoad<float>(ColorGlow, "color_glow");
	CFG::get().addToLoad<float>(onVisibleColorBox, "color_box_visible");
	CFG::get().addToLoad<float>(fovColor, "fovColor");
	CFG::get().addToLoad<float>(menuCheckboxWarningColor, "menuCheckboxWarningColor");
	CFG::get().addToLoad<float>(menuCheckboxesColor, "menuCheckboxesColor");
	CFG::get().addToLoad<float>(menuFrameColor, "menuFrameColor");
	CFG::get().addToLoad<float>(menuCheckedCBoxes, "menuCheckedCBoxes");
	CFG::get().addToLoad<float>(menuChildColor, "menuChildColor");
	CFG::get().addToLoad<float>(menuButtonColor, "menuButtonColor");


	CFG::get().load();
}

void Config::ResetCFG()
{
	BoxEsp = false;
	LineEsp = false;
	HpEsp = false;
	currTypeOfBox = 1;
	currGlowType = 1;
	FOV = 1;
	Smooth = 1;
	drawFOV = false;
	FOV_Outline = false;
	TriggerSafe = false;
	Trigger = false;
	aimDormantCheck = false;
	onVisible = false;
	isVisible = false;
	hpType = 1;
	currBone = 1;
	BHOP = false;
	RCS = false;
	Glow = false;
	TeamEsp = false;
	TeamAim = false;
	RenderbestAimTarget = false;
	FOV_Indicate = false;
	chams = false;
	ColorBox[0] = 0.f;
	ColorBox[1] = 1.f;
	ColorBox[2] = 0.f;
	ColorBox[3] = 1.f;
	onVisibleColorBox[0] = 1.f;
	onVisibleColorBox[1] = 0.f;
	onVisibleColorBox[2] = 0.f;
	onVisibleColorBox[3] = 1.f;
	ColorGlow[0] = 1.f;
	ColorGlow[1] = 0.f;
	ColorGlow[2] = 1.f;
	ColorGlow[3] = 1.f;

	SafeCfg();

}
