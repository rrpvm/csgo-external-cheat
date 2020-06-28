#pragma once
#include <Windows.h>
#include "vec3.h"
#include <iostream>
#include "Singleton.h"

class LocalPlayer : public Singleton <LocalPlayer>
{
protected:	
	int getHealth();
	int getTeam();
	int getFlags();
	vec3 getOrigin();
	vec3 GetviewOffset();
	bool getDormant();
	bool isScoping();

public:
	LocalPlayer() {};
	LocalPlayer(uintptr_t addres) { base = addres; };
	~LocalPlayer() {};
	int health;
	int team;
	int flags;
	int shotsFired;
	bool ownerC4;
	bool inScope;
	bool isDefusing;
	bool isDormant;
	vec3 origin;
	bool spotted = false;
	void Update();
	vec3 viewOffset;
	vec3 getBonePos(int boneId, uintptr_t base);
	uintptr_t getBase() { return base; }
	int GetShotsFired();
	std::string get_weapon_name(int weaponId, bool icons = false);
	const char* get_weapon_name2(int weaponId);
	int getWeaponNameInArray(int weaponId);
	int getIdByName(std::string name);
protected:
	uintptr_t base;
};
enum bones {
	head = 8,
};
struct player_info_t {
	char __pad[0x10];
	char name[32];
};
