#pragma once
#include "Singleton.h"
#include "GlobalVars.h"
#include "../IMGUI/imgui.h"
#include "Config.h"
#include "../IMGUI/imgui_impl_dx9.h"
#include "../IMGUI/imgui_impl_win32.h"
#include <vector>
class menu : public Singleton <menu>
{
public:
	void init();
	void drawMainMenu();
	void endMenuScene();
	void setStyles();
	//LRESULT WINAPI WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	bool showMenu = false;
	WNDPROC wndproc_orig;
	int sizeX = 640;
	int sizeY = 480;
	int currTab = 1;
	void drawAimbotTab();
	void drawAimbotTab2();
	ImFont* fTabs;
	ImFont* weaponTabs;
	ImFont* defFont;
	ImFont* espFont;
	ImDrawList* list;
	void drawVisualSubTab();
	void drawMiscTab();
	void drawMiscTab2();
	void drawCFGTab();
	void drawCFGTab2();
	void drawVisualSubTab2();
	ImDrawList* getDrawList() { return list; };
	int menuType = 1;
private:
	bool isInit = false;
	

};

