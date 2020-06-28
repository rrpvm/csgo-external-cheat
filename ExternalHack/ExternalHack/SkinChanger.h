#pragma once
#include "Singleton.h"
#include <vector>
enum SkinList {
	cherepa = 11,
	bloody_web = 12,
	blaze = 37,
	fade,
	BOOM =	174,
	Emerald	 = 196,
	fireSerpent = 180,
	Asiimov	 = 279,
	DLor = 344,
	Aquamarine = 353,
	HotRod = 445,
	Medusa,
	Metallic_DDPAT = 449,
	NeoNoir = 653,
};

class SkinChanger : public Singleton <SkinChanger>
{
public:
	void doSkinChanger();
	int getStatTrackById(int id);
	int getSkinById(int id);
	int getIdOfSkinByComboList(int id);
	float getWearById(int id);
	std::string getCustomNameById(int id);
private:
};

