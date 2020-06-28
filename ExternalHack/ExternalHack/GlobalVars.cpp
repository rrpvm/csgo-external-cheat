#include "GlobalVars.h"
#include "Player.h"
void GlobalVars::updateEnemyList()
{
	activeEnemyCounter = 0;
	activeFriendsCounter = 0;
	DWORD bufferL = memory::get().read<uintptr_t>(GlobalVars::get().client + GlobalVars::get().ofs.localPlayer);
	localPlayer = new LocalPlayer(bufferL);
	localPlayer->Update();
	for (int i = 0; i < 64; i++)
	{
		DWORD buffer = memory::get().read<uintptr_t>(client + ofs.dwEntityList + i * 0x10);		
		if (buffer != NULL)
		{
			Player* pEntity = new Player(memory::get().read<uintptr_t>(client + ofs.dwEntityList + i * 0x10));
			pEntity->Update();
			if (pEntity->health > 0 && pEntity->team != localPlayer->team && !pEntity->isDormant)//21.06
			{
				EnemyList[activeEnemyCounter] = pEntity;
				activeEnemyCounter++;
			}
			else if (Config::get().TeamEsp)
			{
				if (pEntity->team == localPlayer->team && pEntity->health > 0)
				{
					if (buffer != bufferL)
					{
						FriendList[activeFriendsCounter] = pEntity;
						activeFriendsCounter++;
					}				
				}
				else
				{
					delete pEntity;
				}
			}
			else{
				delete pEntity;
			}					
		}		
	}
}

void GlobalVars::clearEnemyList()
{
	for (int i = 0; i < activeEnemyCounter; i++)
	{
		if(EnemyList[i] != nullptr)
		delete EnemyList[i];
	}
	for (int i = 0; i < 64; i++)
	{
		if(FriendList[i])
		delete FriendList[i];
	}
	delete localPlayer;
}
void GlobalVars::printOffsets()
{
	offsets* base = &ofs;
	std::cout << base << " = link to struct of offsets { \n\n";
	for (int i = 0; i < sizeof(offsets) / sizeof(DWORD); i++)
	{	
		std::cout <<std::hex  << "0x"<< *(uintptr_t*)((uintptr_t)base + i * sizeof(DWORD)) <<  std::dec<<  "\n";
	}
	std::cout << "} \n";
}

Player* GlobalVars::findEntityByBaseInList(uintptr_t entityBase)
{
	for (int i = 0; i < activeEnemyCounter; i++)
	{
		if (EnemyList[i]->getBase() == entityBase)
		{

			return EnemyList[i];
		}
	}
}

