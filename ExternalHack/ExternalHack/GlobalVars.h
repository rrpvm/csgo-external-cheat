#pragma once
#include <Windows.h>
#include "Singleton.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#include <vector>
#include "Player.h"
#include "Memory.h"
#include "Config.h"
/*GVARS.h : Player.h -> Player.h : LocalPlayer.h*/
class GlobalVars : public Singleton <GlobalVars>
{
public:
	uintptr_t client;
	uintptr_t engine;
	uintptr_t vstdlib;
	DWORD pId;
	HANDLE csgo;
	HWND overlayHWND;
	HWND csgoHWND;
	RECT overlayRect;//and csgo rect
	RECT marginRect;
	struct offsets {
		DWORD localPlayer = 0xD3ABEC;
		DWORD dwEntityList = 0x4D4F25C;
		DWORD m_ViewMatrix = 0x4D40BA4;
		DWORD dwGlowObjectManager = 0x5297080;
		DWORD dwForceJump = 0x51F8EF4;
		DWORD dwForceAttack = 0x31807A8;
		DWORD boneMatrix = 0x26A8;
		DWORD dwClientState = 0x58ADD4;
		DWORD dwClientState_ViewAngles = 0x4D88;
		DWORD dwbSendPackets = 0xD415A;
		DWORD clientstate_choked_commands = 0x4D28;
		DWORD m_hActiveWeapon = 0x2EF8;
		DWORD m_iItemDefinitionIndex = 0x2FAA;
		DWORD m_nFallbackStatTrak = 0x31D4;
		DWORD m_szCustomName = 0x303C;
		DWORD m_iItemIDHigh = 0x2FC0;
		DWORD m_nFallbackPaintKit = 0x31C8;
		DWORD m_nFallbackSeed = 0x31CC;
		DWORD m_iEntityQuality = 0x2FAC;
		DWORD m_flFallbackWear = 0x31D0;
		DWORD m_hMyWeapons = 0x2DF8;
		DWORD clientstate_delta_ticks = 0x174;


		DWORD m_iHealth = 0x100;
		DWORD dwFlags = 0x104;
		DWORD m_iTeam = 0xF4;
		DWORD m_vecOrigin = 0x138;	
		DWORD vecViewOffes = 0x108;			
		DWORD m_iGlowIndex = 0xA438;
		DWORD dormant = 0xED;
		DWORD  m_aimPunchAngle = 0x302C;
		DWORD m_iShotsFired = 0xA390;
		DWORD m_viewPunchAngle = 0x3020;		
		DWORD m_iCrosshairId  = 0xB3E4;	
		DWORD b_Spotted = 0x93D;
		DWORD m_bIsScoped = 0x3928;
	} ofs;
	IDirect3DDevice9* pDevice;
	IDirect3D9* g_D3D;
	ID3DXFont* defFont = nullptr;
	Player* EnemyList[64] = { 0 };
	Player* FriendList[64];
	LocalPlayer* localPlayer;
	int activeEnemyCounter = 0;
	int RezX = 0;
	int RezY = 0;
	int activeFriendsCounter = 0;
	void updateEnemyList();
	void clearEnemyList();
	void printOffsets();
	Player* findEntityByBaseInList(uintptr_t entityBase);
};
enum ItemDefinitionIndex
{
	WEAPON_NONE = 0,
	WEAPON_DEAGLE,
	WEAPON_ELITE,
	WEAPON_FIVESEVEN,
	WEAPON_GLOCK,
	WEAPON_AK47 = 7,
	WEAPON_AUG,
	WEAPON_AWP,
	WEAPON_FAMAS,
	WEAPON_G3SG1,
	WEAPON_GALILAR = 13,
	WEAPON_M249,
	WEAPON_M4A1 = 16,
	WEAPON_MAC10,
	WEAPON_P90 = 19,
	WEAPON_MP5SD = 23,
	WEAPON_UMP45,
	WEAPON_XM1014,
	WEAPON_BIZON,
	WEAPON_MAG7,
	WEAPON_NEGEV,
	WEAPON_SAWEDOFF,
	WEAPON_TEC9,
	WEAPON_TASER,
	WEAPON_HKP2000,
	WEAPON_MP7,
	WEAPON_MP9,
	WEAPON_NOVA,
	WEAPON_P250,
	WEAPON_SCAR20 = 38,
	WEAPON_SG556,
	WEAPON_SSG08,
	WEAPON_KNIFEGG,
	WEAPON_KNIFE,
	WEAPON_FLASHBANG,
	WEAPON_HEGRENADE,
	WEAPON_SMOKEGRENADE,
	WEAPON_MOLOTOV,
	WEAPON_DECOY,
	WEAPON_INCGRENADE,
	WEAPON_C4,
	WEAPON_HEALTHSHOT = 57,
	WEAPON_KNIFE_T = 59,
	WEAPON_M4A1_SILENCER,
	WEAPON_USP_SILENCER,
	WEAPON_CZ75A = 63,
	WEAPON_REVOLVER,
	WEAPON_TAGRENADE = 68,
	WEAPON_FISTS,
	WEAPON_BREACHCHARGE,
	WEAPON_TABLET = 72,
	WEAPON_MELEE = 74,
	WEAPON_AXE,
	WEAPON_HAMMER,
	WEAPON_SPANNER = 78,
	WEAPON_KNIFE_GHOST = 80,
	WEAPON_FIREBOMB,
	WEAPON_DIVERSION,
	WEAPON_FRAG_GRENADE,
	WEAPON_BAYONET = 500,
	WEAPON_KNIFE_FLIP = 505,
	WEAPON_KNIFE_GUT,
	WEAPON_KNIFE_KARAMBIT,
	WEAPON_KNIFE_M9_BAYONET,
	WEAPON_KNIFE_TACTICAL,
	WEAPON_KNIFE_FALCHION = 512,
	WEAPON_KNIFE_SURVIVAL_BOWIE = 514,
	WEAPON_KNIFE_BUTTERFLY,
	WEAPON_KNIFE_PUSH,
	WEAPON_KNIFE_URSUS = 519,
	WEAPON_KNIFE_GYPSY_JACKKNIFE,
	WEAPON_KNIFE_STILETTO = 522,
	WEAPON_KNIFE_WIDOWMAKER,
	GLOVE_STUDDED_BLOODHOUND = 5027,
	GLOVE_T_SIDE = 5028,
	GLOVE_CT_SIDE = 5029,
	GLOVE_SPORTY = 5030,
	GLOVE_SLICK = 5031,
	GLOVE_LEATHER_WRAP = 5032,
	GLOVE_MOTORCYCLE = 5033,
	GLOVE_SPECIALIST = 5034,
	GLOVE_HYDRA = 5035
};
