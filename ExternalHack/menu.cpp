#include "menu.h"
#include <Windows.h>
#include "fakelag.h"
#include "SkinChanger.h"
LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (menu::get().showMenu && ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
	{
		return true;
	}
	return CallWindowProc(menu::get().wndproc_orig, hwnd, msg, wParam, lParam);
}
void menu::init()
{
	if (!isInit)
	{
		wndproc_orig = (WNDPROC)SetWindowLongPtr(FindWindowA(NULL, "ExternalByRoman"), GWLP_WNDPROC, (LONG_PTR)WndProc);
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init(FindWindowA(NULL, "ExternalByRoman"));
		ImGui_ImplDX9_Init(GlobalVars::get().pDevice);		
		fTabs = io.Fonts->AddFontFromFileTTF("Untitled1.ttf", 62.0f);
		weaponTabs = io.Fonts->AddFontFromFileTTF("undefeated.ttf", 12.0f);
		defFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.TTF", 16.0f);
		isInit = true;
	}
}

void menu::drawMainMenu()
{
	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		showMenu = !showMenu;
		if (showMenu)
		{
			SetWindowLongPtr(GlobalVars::get().overlayHWND, GWL_EXSTYLE, WS_EX_LAYERED);
		}
		else SetWindowLongPtr(GlobalVars::get().overlayHWND, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TRANSPARENT);
	}

	init();
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	if (showMenu)
	{
		if (isInit)
		{
			setStyles();//

			ImGui::SetNextWindowSize(ImVec2(sizeX, sizeY), ImGuiCond_Always);
			ImGuiStyle& style = ImGui::GetStyle();
			ImGui::PushFont(defFont);
			ImGui::Begin("Menu", &showMenu, ImGuiCond_Always | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);
			{
				ImGui::BeginChild("OVER_MENU", ImVec2(640, 33), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
				{
					ImGui::SameLine(640 / 2 - ImGui::CalcTextSize("ExternalHack  status:(UNDETECT) V1.02 pre-release").x / 2);
					ImGui::SetCursorPosY(10);
					ImGui::Text("ExternalHack  status:(UNDETECT) V1.02 pre-release");
				}
				ImGui::EndChild();
				ImGui::BeginChild("menu-tabs", ImVec2(165, sizeY), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_AlwaysAutoResize);
				{
					ImGui::NewLine();
					ImGui::PushFont(fTabs);
					currTab == 2 ? style.FrameBorderSize = 1.f : style.FrameBorderSize = 0.f;
					currTab == 2 ? style.Colors[ImGuiCol_Button] = ImVec4(.2f, .2f, .2f, 1.f) : style.Colors[ImGuiCol_Button] = ImVec4(.1f, .1f, .1f, 1.f);
					if (ImGui::Button("a", ImVec2(165, sizeY / 5)))
					{
						currTab = 2;
					}
					currTab == 1 ? style.FrameBorderSize = 1.f : style.FrameBorderSize = 0.f;
					currTab == 1 ? style.Colors[ImGuiCol_Button] = ImVec4(.2f, .2f, .2f, 1.f) : style.Colors[ImGuiCol_Button] = ImVec4(.1f, .1f, .1f, 1.f);
					if (ImGui::Button("b", ImVec2(165, sizeY / 5)))
					{
						currTab = 1;
					}
					currTab == 3 ? style.FrameBorderSize = 1.f : style.FrameBorderSize = 0.f;
					currTab == 3 ? style.Colors[ImGuiCol_Button] = ImVec4(.2f, .2f, .2f, 1.f) : style.Colors[ImGuiCol_Button] = ImVec4(.1f, .1f, .1f, 1.f);
					if (ImGui::Button("f", ImVec2(165, sizeY / 5)))
					{
						currTab = 3;
					}
					currTab == 4 ? style.FrameBorderSize = 1.f : style.FrameBorderSize = 0.f;
					currTab == 4 ? style.Colors[ImGuiCol_Button] = ImVec4(.2f, .2f, .2f, 1.f) : style.Colors[ImGuiCol_Button] = ImVec4(.1f, .1f, .1f, 1.f);
					if (ImGui::Button("d", ImVec2(165, sizeY / 5)))
					{
						currTab = 4;
					}
					style.FrameBorderSize = 1.f;
					ImGui::PopFont();
				}
				ImGui::EndChild();
				ImGui::SameLine();
				ImGui::BeginChild("sub-tabs", ImVec2(sizeX - 165, 480), false, ImGuiWindowFlags_NoScrollbar);
				{
					switch (currTab)
					{
					case 1: {
						drawVisualSubTab();
						break;
					}
					case 2: {
						drawAimbotTab();
						break;
					}
					case 3: {
						drawMiscTab();
						break;
					}
					case 4: {
						drawCFGTab();
						break;
					}

					}
				}
			}
		}
		
		
	}
	ImGui::EndChild();
}

void menu::endMenuScene()
{
	ImGui::End();
	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void menu::setStyles()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 255.f;
	style.ChildBorderSize = 0.8f;
	style.DisplaySafeAreaPadding = ImVec2(0, 0);
	style.DisplayWindowPadding = ImVec2(0, 0);
	style.FramePadding = ImVec2(8, 2);
	style.IndentSpacing = 10.f;
	style.ItemInnerSpacing = ImVec2(12, 0);
	style.ItemSpacing = ImVec2(10,0);
	style.WindowPadding = ImVec2(0, 0);
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.FrameBorderSize = 0.f;
	style.FrameRounding =2.0f;
	style.WindowRounding = 5.0f;

	style.Colors[ImGuiCol_Button] = ImVec4(.1f, .1f, .1f,1.f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(.1f, .1f, .1f,1.f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(.2f, .2f, .2f,1.f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(Config::get().menuFrameColor[0], Config::get().menuFrameColor[1], Config::get().menuFrameColor[2], Config::get().menuFrameColor[3]);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(Config::get().menuChildColor[0], Config::get().menuChildColor[1], Config::get().menuChildColor[2], Config::get().menuChildColor[3]);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(Config::get().menuCheckboxesColor[0], Config::get().menuCheckboxesColor[1], Config::get().menuCheckboxesColor[2], Config::get().menuCheckboxesColor[3]);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(Config::get().menuCheckedCBoxes[0] + .1f, Config::get().menuCheckedCBoxes[1] + .1f, Config::get().menuCheckedCBoxes[2] + .1f, Config::get().menuCheckedCBoxes[3]);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(Config::get().menuCheckedCBoxes[0] + .2f, Config::get().menuCheckedCBoxes[1] + .2f, Config::get().menuCheckedCBoxes[2] + .2f, Config::get().menuCheckedCBoxes[3]);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(Config::get().menuCheckedCBoxes[0], Config::get().menuCheckedCBoxes[1], Config::get().menuCheckedCBoxes[2], Config::get().menuCheckedCBoxes[3]);


	
	style.Colors[ImGuiCol_Tab] = ImVec4(.2f, .2f, .2f, 1.f);
	style.Colors[ImGuiCol_PopupBg] =  ImVec4(.2f, .2f, .2f, 1.f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(.5f, .2f, .2f, 1.f);
	style.Colors[ImGuiCol_Header] = ImVec4(.5f, .2f, .2f, 1.f);
	style.FrameBorderSize = .1f;
}

void menu::drawAimbotTab()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameBorderSize = 0.f;
	style.PopupBorderSize = 0.f;
	style.TabBorderSize = 0.f;
	style.WindowPadding = ImVec2(9, 2);
	style.ItemInnerSpacing = ImVec2(9, 15);
	style.ItemSpacing = ImVec2(5, 9);

	ImGui::BeginChild("Functions", ImVec2((sizeX - 165) / 2, 480), true);
	ImGui::SetCursorPosY(10);
	ImGui::SetCursorPosX((sizeX - 165) / 4 - ImGui::CalcTextSize("Functions").x);
	ImGui::Text("Functions");
	ImGui::SetCursorPosY(50);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(Config::get().menuCheckboxWarningColor[0], Config::get().menuCheckboxWarningColor[1], Config::get().menuCheckboxWarningColor[2], Config::get().menuCheckboxWarningColor[3]);
	ImGui::Checkbox("Angle Aimbot", &Config::get().WriteAimBot);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(Config::get().menuCheckboxesColor[0], Config::get().menuCheckboxesColor[1], Config::get().menuCheckboxesColor[2], Config::get().menuCheckboxesColor[3]);
	ImGui::Checkbox("Cursor Aimbot", &Config::get().SafeAimBot);
	ImGui::Checkbox("dormant aim", &Config::get().aimDormantCheck);
	ImGui::Checkbox("RCS", &Config::get().RCS);
	ImGui::SetNextItemWidth((sizeX - 165) / 2 - 70);
	ImGui::SetCursorPosX(9); ImGui::SliderInt("FOV", &Config::get().FOV, 1, 120, "%d");
	ImGui::SetCursorPosX(9);
	ImGui::SetNextItemWidth((sizeX - 165) / 2 - 70);
	ImGui::SliderInt("SMOOTH", &Config::get().Smooth, 1, 100, "%d");
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("Other", ImVec2((sizeX - 165) / 2, 480), true);
	ImGui::SetNextItemWidth(120);
	ImGui::SetCursorPosY(50);;
	ImGui::Combo("aim bone", &Config::get().currBone, Config::get().aim_preference, sizeof(Config::get().aim_preference) / sizeof(Config::get().aim_preference[0]), sizeof(Config::get().aim_preference) / sizeof(Config::get().aim_preference[0]));
	ImGui::Button("Aim key(coming soon)");
	if (ImGui::Checkbox("FakeLag", &Config::get().fake_lag)) {
		static bool once = true;
		if (once) {
			if (!Config::get().fake_lag) { fakelag::get().shutdown(); once = false; }
		}
		if (Config::get().fake_lag)
		{
			once = true;
		}
	}
	ImGui::SliderInt("factor", &Config::get().factor, 1, 14, "%d");
	ImGui::Checkbox("indicate chocked", &Config::get().fakelag_indicator);
	ImGui::EndChild();
	style.WindowPadding = ImVec2(0, 0);
	style.ItemInnerSpacing = ImVec2(1, 1);
	style.ItemSpacing = ImVec2(1, 1);
}

void menu::drawVisualSubTab()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameBorderSize = 0.f;
	style.PopupBorderSize = 0.f;
	style.TabBorderSize = 0.f;
	style.WindowPadding = ImVec2(9, 0);
	ImGui::BeginChild("checkboxes", ImVec2((sizeX - 165)/2 - 50, 480),true);
	ImGui::SameLine();
	ImGui::SetCursorPosY(20);
	ImGui::Text("Functions");	
	ImGui::NewLine();
	style.ItemInnerSpacing = ImVec2(9, 15);
	style.ItemSpacing = ImVec2(5, 5);
	
	ImGui::Checkbox("BoxEsp", &Config::get().BoxEsp);
	ImGui::Checkbox("Line Esp", &Config::get().LineEsp);
	ImGui::Checkbox("Hp Esp", &Config::get().HpEsp);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(Config::get().menuCheckboxWarningColor[0], Config::get().menuCheckboxWarningColor[1], Config::get().menuCheckboxWarningColor[2], Config::get().menuCheckboxWarningColor[3]);
	ImGui::Checkbox("Glow", &Config::get().Glow);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(Config::get().menuCheckboxesColor[0], Config::get().menuCheckboxesColor[1], Config::get().menuCheckboxesColor[2], Config::get().menuCheckboxesColor[3]);
	ImGui::Checkbox("Team Esp", &Config::get().TeamEsp);
	ImGui::Checkbox("only visible", &Config::get().isVisible);
	ImGui::Checkbox("on visible", &Config::get().onVisible);
	ImGui::Checkbox("drawAimTarget", &Config::get().RenderbestAimTarget);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(Config::get().menuCheckboxWarningColor[0], Config::get().menuCheckboxWarningColor[1], Config::get().menuCheckboxWarningColor[2], Config::get().menuCheckboxWarningColor[3]);
	ImGui::Checkbox("fake chams", &Config::get().chams);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(Config::get().menuCheckboxesColor[0], Config::get().menuCheckboxesColor[1], Config::get().menuCheckboxesColor[2], Config::get().menuCheckboxesColor[3]);
	ImGui::Checkbox("show head pos", &Config::get().showHeadPos);
	ImGui::Checkbox("draw FOV", &Config::get().drawFOV);
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("other", ImVec2((sizeX - 165) / 2 + 50, 480), true);
	ImGui::SetCursorPos(ImVec2(ImGui::CalcItemWidth()/1.6,20));
	ImGui::Text("other");
	ImGui::NewLine();
	ImGui::ColorPicker4("color", Config::get().ColorBox, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_InputRGB);
	ImGui::ColorPicker4("glow color", Config::get().ColorGlow, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_InputRGB);
	ImGui::ColorPicker4("Box color on visible", Config::get().onVisibleColorBox, ImGuiColorEditFlags_NoInputs);
	ImGui::ColorPicker4("Fov Indicate color", Config::get().fovColor, ImGuiColorEditFlags_NoInputs);
	ImGui::SetNextItemWidth(120);
	ImGui::Combo("box type", &Config::get().currTypeOfBox, Config::get().boxType, sizeof(Config::get().boxType) / sizeof(Config::get().boxType[0]), sizeof(Config::get().boxType) / sizeof(Config::get().boxType[0]));
	ImGui::SetNextItemWidth(120);
	ImGui::Combo("hp pos", &Config::get().hpType,Config::get().hptype, sizeof(Config::get().hptype) / sizeof(Config::get().hptype[0]),sizeof(Config::get().hptype) / sizeof(Config::get().hptype[0]));
	ImGui::SetNextItemWidth(120);
	ImGui::Combo("Glow Type", &Config::get().currGlowType, Config::get().glowType, sizeof(Config::get().glowType) / sizeof(Config::get().glowType[0]), sizeof(Config::get().glowType) / sizeof(Config::get().glowType[0]));
	ImGui::Checkbox("only outline FOV", &Config::get().FOV_Outline);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(Config::get().menuCheckboxWarningColor[0], Config::get().menuCheckboxWarningColor[1], Config::get().menuCheckboxWarningColor[2], Config::get().menuCheckboxWarningColor[3]);
	ImGui::Checkbox("Indicate player on FOV", &Config::get().FOV_Indicate);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(.2f, .2f, .2f, 1.f);
	ImGui::Checkbox("EnemyWeapon", &Config::get().enemyWeapon);
	ImGui::Checkbox("recoil crz", &Config::get().recoilCrz);
	ImGui::SliderInt("glow alpha", &Config::get().glowThikness, 1, 10, "%d");
	ImGui::Text("coming soon (update v2.0)");
	ImGui::EndChild();
	style.WindowPadding = ImVec2(0, 0);
	style.ItemInnerSpacing = ImVec2(1, 1);
	style.ItemSpacing = ImVec2(1, 1);

}

void menu::drawMiscTab()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameBorderSize = 0.f;
	style.PopupBorderSize = 0.f;
	style.TabBorderSize = 0.f;
	style.WindowPadding = ImVec2(9, 2);
	style.ItemInnerSpacing = ImVec2(9, 15);
	style.ItemSpacing = ImVec2(5, 9);

	ImGui::BeginChild("Functions", ImVec2((sizeX - 165) / 2, 480), true);
	ImGui::SetCursorPosY(10);
	ImGui::SetCursorPosX((sizeX - 165) / 4 - ImGui::CalcTextSize("Functions").x);
	ImGui::Text("Functions");
	ImGui::SetCursorPosY(50);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(Config::get().menuCheckboxWarningColor[0], Config::get().menuCheckboxWarningColor[1], Config::get().menuCheckboxWarningColor[2], Config::get().menuCheckboxWarningColor[3]);
	ImGui::Checkbox("Triggerbot", &Config::get().Trigger);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(Config::get().menuCheckboxesColor[0], Config::get().menuCheckboxesColor[1], Config::get().menuCheckboxesColor[2], Config::get().menuCheckboxesColor[3]);
	ImGui::Checkbox("Triggerbot(safe)", &Config::get().safeTrigger);
	ImGui::Checkbox("Bunnyhop", &Config::get().BHOP);
	ImGui::SliderInt("delay btw shots", &Config::get().delayBetweenShots, 1, 1000, "%d");
	ImGui::Text("coming soon (update v2.0)");
	ImGui::EndChild();
	
	style.WindowPadding = ImVec2(0, 0);
	style.ItemInnerSpacing = ImVec2(1, 1);
	style.ItemSpacing = ImVec2(1, 1);
}

void menu::drawCFGTab()
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.FrameBorderSize = 0.f;
	style.PopupBorderSize = 0.f;
	style.TabBorderSize = 0.f;
	style.WindowPadding = ImVec2(9, 2);
	style.ItemInnerSpacing = ImVec2(9, 15);
	style.ItemSpacing = ImVec2(5, 9);
	ImGui::BeginChild("Functions", ImVec2((sizeX - 165) / 2, 480), true);
	if (ImGui::Button("save"))
	{
		Config::get().SafeCfg();
	}
	if (ImGui::Button("Load"))
	{
		Config::get().LoadCfg();
	}
	if (ImGui::Button("reset"))
	{
		Config::get().ResetCFG();
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("SkinChanger", ImVec2((sizeX - 165) / 2, 480), true);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(Config::get().menuCheckboxWarningColor[0], Config::get().menuCheckboxWarningColor[1], Config::get().menuCheckboxWarningColor[2], Config::get().menuCheckboxWarningColor[3]);
	ImGui::Checkbox("SkinChanger", &Config::get().skingChanger);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(Config::get().menuCheckboxesColor[0], Config::get().menuCheckboxesColor[1], Config::get().menuCheckboxesColor[2], Config::get().menuCheckboxesColor[3]);
	if (ImGui::Button("update"))
	{
		Config::get().cfgWeapons[Config::get().currComboWeaponSkin].skinId = SkinChanger::get().getIdOfSkinByComboList(Config::get().cfgWeapons[Config::get().currComboWeaponSkin].currComboSelect);

		uintptr_t clientState = memory::get().read<uintptr_t>(GlobalVars::get().engine + GlobalVars::get().ofs.dwClientState);		
		memory::get().write<int>(clientState + 0x174, -1);
		memory::get().write<int>(clientState + 0x174, -1);
		memory::get().write<int>(clientState + 0x174, -1);
	}
	static bool once = true;
	if (once)
	{		
		for (int i = 0; i < Config::get().skinItems; i++)
		{
			Config::get().cfgWeapons[i].weaponName = LocalPlayer::get().get_weapon_name2(i);
			Config::get().cfgWeapons[i].skinId = fade;
			Config::get().cfgWeapons[i].wear = 0.01f;
			Config::get().cfgWeapons[i].seed = 0.01f;
			Config::get().cfgWeapons[i].statTrack = 1337;
			Config::get().cfgWeapons[i].customName.resize(32);
			Config::get().cfgWeapons[i].customName = "SUKA TI EBENAYA";
		}
		once = false;
	}
	ImGui::Combo("weapons", &Config::get().currComboWeaponSkin, Config::get().skinchanger_skins, sizeof(Config::get().skinchanger_skins) / sizeof(Config::get().skinchanger_skins[0]), sizeof(Config::get().skinchanger_skins) / sizeof(Config::get().skinchanger_skins[0]));
	ImGui::Combo("skins", &Config::get().cfgWeapons[Config::get().currComboWeaponSkin].currComboSelect, Config::get().skins, sizeof(Config::get().skins) / sizeof(Config::get().skins[0]), sizeof(Config::get().skins) / sizeof(Config::get().skins[0]));
	ImGui::SliderFloat("wear", &Config::get().cfgWeapons[Config::get().currComboWeaponSkin].wear, 0.f, 1.f, "%f");

	ImGui::ColorPicker4("checkboxes color", Config::get().menuCheckboxesColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_InputRGB);
	ImGui::ColorPicker4("checkboxes active color", Config::get().menuCheckedCBoxes, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_InputRGB);
	ImGui::ColorPicker4("checkboxes warning color", Config::get().menuCheckboxWarningColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_InputRGB);
	ImGui::ColorPicker4("frame color", Config::get().menuFrameColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_InputRGB);
	ImGui::ColorPicker4("childs color", Config::get().menuChildColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_InputRGB);
	ImGui::ColorPicker4("button color", Config::get().menuButtonColor, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_InputRGB);
	ImGui::EndChild();


	style.WindowPadding = ImVec2(0, 0);
	style.ItemInnerSpacing = ImVec2(1, 1);
	style.ItemSpacing = ImVec2(1, 1);
}



extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
