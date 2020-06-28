#include <iostream>
#include "Memory.h"
#include "GlobalVars.h"
#include "Renderer.h"
#include <thread>
#include <chrono>
#include <Dwmapi.h> 
#include "AimBot.h"
#include "RCS.h"
#include "Bunnyhope.h"
#include "Triggerbot.h"
#include "fakelag.h"
#include "SkinChanger.h"
#include <fstream>
#pragma comment (lib, "Dwmapi.lib")

static bool shouldDestroy = false;//if true -> destroy window
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
int main()
{
	std::cout << "ExternalHack by Roman (23.06.2020); \n";
	std::cout << "Statuc << UNDETECT; \n";
	std::cout << "Waiting for csgo ... \n";
	Sleep(2000);
	GlobalVars::get().pId = memory::get().getProcessId("csgo.exe");
	if(GlobalVars::get().pId != 0)
	std::cout << "Process Id of CSGO is << " << GlobalVars::get().pId << " << \n";
	else {
		while (GlobalVars::get().pId == 0)
		{
			GlobalVars::get().pId = memory::get().getProcessId("csgo.exe");
			GlobalVars::get().pId == 0 ? std::cout << "Waiting for csgo ... \n" : std::cout << "CSGO FOUND, procID = " << GlobalVars::get().pId << " \n";
			Sleep(2500);
		}
	}
	GlobalVars::get().client = memory::get().GetBase("client.dll");
	std::cout << "Search for " << " client.dll " << " in csgo.exe" << " addres  = " << GlobalVars::get().client << "\n";
	GlobalVars::get().engine = memory::get().GetBase("engine.dll");
	std::cout << "Search for " << " engine.dll " << " in csgo.exe" << " addres  = " << GlobalVars::get().engine << "\n";
	GlobalVars::get().vstdlib = memory::get().GetBase("vstdlib.dll");
	GlobalVars::get().csgo = OpenProcess(PROCESS_ALL_ACCESS, 0, GlobalVars::get().pId);
	try {
		std::ifstream file;
		file.open("cfg.cfg", std::ios::out);
		if (file.is_open())
		{
			Config::get().LoadCfg();
		}
		else {
			throw "cfg doesnt exist ";
		}
	}
	catch (const char* error)
	{
		std::cout << error << "\n";
	}
	GlobalVars::get().printOffsets();
	WinMain(0, 0, 0, 1);
	fakelag::get().shutdown();
	CloseHandle(GlobalVars::get().csgo);
	std::cout << "cheat going to sleep \n\n zzzzzzz";
}
const MARGINS  margin = { 0, 0, GlobalVars::get().overlayRect.right - GlobalVars::get().overlayRect.left , GlobalVars::get().overlayRect.bottom - GlobalVars::get().overlayRect.top };
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_DESTROY:
		GlobalVars::get().pDevice->Release();
		exit(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
bool initD3D(HWND hwnd, IDirect3DDevice9** pDevice,int x, int y)
{

	GlobalVars::get().g_D3D = Direct3DCreate9(D3D_SDK_VERSION);
	D3DPRESENT_PARAMETERS PresParam;      
	ZeroMemory(&PresParam, sizeof(PresParam));   
	HRESULT hr = NULL;              

	D3DDISPLAYMODE DisplayMode;          
	hr = GlobalVars::get().g_D3D->GetAdapterDisplayMode(   
		D3DADAPTER_DEFAULT,              
		&DisplayMode);                

	if (FAILED(hr))             
		return false;
	PresParam.Windowed = true;
	PresParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
	PresParam.BackBufferFormat = D3DFMT_A8R8G8B8;
	PresParam.BackBufferWidth =x;
	PresParam.BackBufferHeight = y;
	PresParam.hDeviceWindow =hwnd;
	PresParam.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	PresParam.EnableAutoDepthStencil = true;
	PresParam.AutoDepthStencilFormat = D3DFMT_D16;

	hr = GlobalVars::get().g_D3D->CreateDevice(       
		D3DADAPTER_DEFAULT,             
		D3DDEVTYPE_HAL,               
		hwnd,    //overlayHWND             
		D3DCREATE_HARDWARE_VERTEXPROCESSING,   
		&PresParam,                 
		pDevice);            
	if (!GlobalVars::get().defFont)
	{
		D3DXCreateFont(GlobalVars::get().pDevice, 14, 0, 0, 0, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Arial", &GlobalVars::get().defFont);
	}	
	static bool once = false;
	if (SUCCEEDED(hr)) {

		if (!once) {
			std::cout << "created PDEVICE \n";
			once = true;
		}
		return true;
	}	
}
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	auto f = GetModuleHandle(NULL);
	WNDCLASSEX wc;
	wc = { 0 };
	wc.cbSize = sizeof(wc);

	//Create our window class
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = HBRUSH(RGB(0, 0, 0));
	wc.lpszClassName = "ExternalDLC";
	wc.hIconSm = NULL;
	if (!RegisterClassEx(&wc)) 
	{

		MessageBox(NULL, "Can`t register window class", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	GlobalVars::get().csgoHWND = FindWindowA(NULL, "Counter-Strike: Global Offensive");
	GetClientRect(GlobalVars::get().csgoHWND,&GlobalVars::get().overlayRect);
	GetWindowRect(GlobalVars::get().csgoHWND,&GlobalVars::get().marginRect);
	GlobalVars::get().RezX = GlobalVars::get().marginRect.right - GlobalVars::get().marginRect.left;
	GlobalVars::get().RezY = GlobalVars::get().overlayRect.bottom - GlobalVars::get().overlayRect.top;

	float diffY = GlobalVars::get().RezY - (GlobalVars::get().marginRect.bottom - GlobalVars::get().marginRect.top);
	
	GlobalVars::get().overlayHWND = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT ,
		"ExternalDLC",
		"ExternalByRoman",
		WS_POPUP | WS_VISIBLE,
		GlobalVars::get().marginRect.left,                      //Позиция окна по оси Х // -(minus)diff
		GlobalVars::get().marginRect.top - diffY,                      //Позиция окна по оси У
		GlobalVars::get().RezX,                //Ширина окна
		GlobalVars::get().RezY,              //Высота окна
		NULL, 
		NULL, 
		f, //getmodulehandle(NULL)
		NULL); 

	MARGINS marg = { GlobalVars::get().marginRect.left ,GlobalVars::get().marginRect.top - diffY, GlobalVars::get().RezX ,GlobalVars::get().RezY };
	SetForegroundWindow(GlobalVars::get().csgoHWND);
	DwmExtendFrameIntoClientArea(GlobalVars::get().overlayHWND, &marg);
	SetLayeredWindowAttributes(GlobalVars::get().overlayHWND, RGB(0, 0, 0), 255, LWA_ALPHA);
	ShowWindow(GlobalVars::get().overlayHWND, SW_SHOW);
	SetWindowPos(GlobalVars::get().csgoHWND, GlobalVars::get().overlayHWND, GlobalVars::get().marginRect.left, GlobalVars::get().marginRect.top - diffY, GlobalVars::get().RezX, GlobalVars::get().RezY, SWP_NOMOVE | SWP_NOSIZE);
	if (GlobalVars::get().overlayHWND == NULL)
	{
		MessageBox(NULL, "Can`t create window", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	if (!initD3D(GlobalVars::get().overlayHWND, &GlobalVars::get().pDevice, GlobalVars::get().RezX, GlobalVars::get().RezY)) {
		MessageBox(NULL, "Can`t create 3D", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}


	SetWindowPos(GlobalVars::get().overlayHWND, HWND_TOPMOST, GlobalVars::get().marginRect.left, GlobalVars::get().marginRect.top - diffY, GlobalVars::get().RezX, GlobalVars::get().RezY, SWP_NOMOVE | SWP_NOSIZE);//del swp_nomove and etc
	MSG msg;	
	ZeroMemory(&msg, sizeof(msg));
	while (!shouldDestroy)
	{
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))//Получаем сообщения
		{
			TranslateMessage(&msg);          //Обрабатываем сообщения
			DispatchMessage(&msg);          //Обрабатываем сообщения
		}
		else {
			static DWORD now = GetTickCount();
			if (GetTickCount() - now > 30000)
			{	
			/*	now = GetTickCount();
				GlobalVars::get().csgoHWND = FindWindowA(NULL, "Counter-Strike: Global Offensive");//21.06
				GetClientRect(GlobalVars::get().csgoHWND, &GlobalVars::get().overlayRect);
				GetWindowRect(GlobalVars::get().csgoHWND, &GlobalVars::get().marginRect);
				GlobalVars::get().RezX = GlobalVars::get().marginRect.right - GlobalVars::get().marginRect.left;
				GlobalVars::get().RezY = GlobalVars::get().overlayRect.bottom - GlobalVars::get().overlayRect.top;

				/*float diffY = GlobalVars::get().RezY - (GlobalVars::get().marginRect.bottom - GlobalVars::get().marginRect.top);
				SetWindowPos(GlobalVars::get().overlayHWND, HWND_TOPMOST,GlobalVars::get().marginRect.left, GlobalVars::get().marginRect.top - diffY, GlobalVars::get().RezX, GlobalVars::get().RezY,0);//del swp_nomove and etc
				MARGINS marg = { GlobalVars::get().marginRect.left, GlobalVars::get().marginRect.top - diffY, GlobalVars::get().RezX, GlobalVars::get().RezY };
				DwmExtendFrameIntoClientArea(GlobalVars::get().overlayHWND, &marg);
				GlobalVars::get().pDevice->Present(0, 0, 0, 0);
				GlobalVars::get().pDevice->Release();
				initD3D(GlobalVars::get().overlayHWND, &GlobalVars::get().pDevice, GlobalVars::get().RezX, GlobalVars::get().RezY); */
			}	

			GlobalVars::get().csgoHWND = FindWindowA(NULL, "Counter-Strike: Global Offensive");//21.06
			if (GlobalVars::get().csgoHWND == NULL)shouldDestroy = true;
			HWND hwnd = GetForegroundWindow();

			if (GlobalVars::get().csgoHWND == hwnd || GlobalVars::get().overlayHWND == hwnd) {//21.06 UPDATED
				GlobalVars::get().updateEnemyList();				
				if (Config::get().WriteAimBot)AimBot::get().DoAimWrite();
				if (Config::get().skingChanger)SkinChanger::get().doSkinChanger();
				Renderer::get().drawFrames();
				if (Config::get().fake_lag)fakelag::get().doFakeLag();
				if (Config::get().Trigger || Config::get().safeTrigger)Triggerbot::get().doTriggerbot();		
			//	if (Config::get().SafeAimBot)AimBot::get().doAimSafe();
				if (Config::get().RCS)RCS::get().doRCS();
				if (Config::get().BHOP)Bunnyhope::get().WriteBHOP();
				
				GlobalVars::get().clearEnemyList();
			}//21.06
			else {
				GlobalVars::get().pDevice->BeginScene();
				GlobalVars::get().pDevice->Clear(0,0,D3DCLEAR_TARGET, D3DCOLOR_RGBA(0,0,0,0), 1.f, 0);
				menu::get().showMenu = false;
				GlobalVars::get().pDevice->EndScene();
				GlobalVars::get().pDevice->Present(0, 0, 0, 0);				
			}
		}
		if (GetAsyncKeyState(VK_DELETE))
		{
			fakelag::get().shutdown();
			shouldDestroy = true;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return msg.wParam;;
}
