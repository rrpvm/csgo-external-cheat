#include "AimBot.h"
#include "Renderer.h"
#include "LocalPlayer.h"
vec3 AimBot::GetClosestTarget()//by crosshair
{
	float max = FLT_MAX;
	float difference = -FLT_MAX;
	float differnceY = 0;
	clientState = memory::get().read<uintptr_t>(GlobalVars::get().engine + GlobalVars::get().ofs.dwClientState);
	viewAnglesAddres = (clientState + GlobalVars::get().ofs.dwClientState_ViewAngles);
	int index = 0;
	OUT_ANGLES = { ZEROTARGET, ZEROTARGET, ZEROTARGET };
	for (int i = 0; i < GlobalVars::get().activeEnemyCounter; i++)
	{
		vec3 origin = GlobalVars::get().EnemyList[i]->origin;
		vec3 localPlayerOrigin = GlobalVars::get().localPlayer->origin;
		vec3 myPOS = localPlayerOrigin + GlobalVars::get().localPlayer->viewOffset;
		vec3 viewAngles = memory::get().read<vec3>(viewAnglesAddres);
		vec3 bonePos = GlobalVars::get().EnemyList[i]->getBonePos(Config::get().boneList[Config::get().currBone], GlobalVars::get().EnemyList[i]->getBase());
		vec2 targetAng = calculateAimAngle(myPOS, bonePos);
		float pitch = targetAng.x;
		float yaw = targetAng.y;
		
		if (viewAngles.x <= 0 && pitch <= 0)
		{
			differnceY = abs(viewAngles.x - pitch);
		}
		else if (viewAngles.x <= 0 && pitch > 0)
		{
			differnceY = abs(viewAngles.x) + pitch;
		}
		else if (viewAngles.x >= 0 && pitch < 0)
		{
			differnceY = viewAngles.x + abs(pitch);
		}
		else if (viewAngles.x >= 0 && pitch > 0)
		{
			differnceY = abs(viewAngles.x - pitch);
		}

		difference = 9999999;
		if (viewAngles.y >= 0 && yaw >= 0)
		{
			difference = abs(viewAngles.y - yaw);
		}
		else if (viewAngles.y >= 0 && yaw <= 0)
		{
			float buffer = abs(-180.f - yaw);// == -180 + abs(yaw)
			float buffer2 = 180.f - viewAngles.y;
			if (buffer + buffer2 <= Config::get().FOV / 3.f)
			{
				difference = abs(viewAngles.y + yaw);
			}
			else {
				difference = viewAngles.y - yaw;// -(-yaw) = + abs(yaw)
			}		
		}
		else if (viewAngles.y <= 0 && yaw >= 0)
		{
			/*BUFF1, BUFF2, BUFF3*/
			float buffer = 180.f - yaw;// == -180 + abs(yaw)
			float buffer2 = abs(-180.f - viewAngles.y);
			if (buffer + buffer2 <= Config::get().FOV / 3.f)
			{
				difference = abs(viewAngles.y + yaw);
			}
			else {
			difference = yaw - viewAngles.y; // - (-view) == +abs(view)
			}
		}
		else if (viewAngles.y <= 0 && yaw <= 0)
		{
			difference = abs(abs(viewAngles.y) - abs(yaw));
		}
		float toCompare = GlobalVars::get().localPlayer->inScope ? Config::get().FOV /7.5f : Config::get().FOV / 3.f;
	
		if (difference <= toCompare && differnceY <= toCompare)
		{
			GlobalVars::get().EnemyList[i]->onFov = true;
			if (difference < max)
			{
				max = difference;
				index = i;
				OUT_ANGLES = { pitch, yaw, (float)i };
			}
			/*if (diff2 <= Config::get().FOV / 3.f) {
				max = diff2;
				index = i;
				OUT_ANGLES = { pitch, yaw, (float)i };
			}*/
		}
		else {
			continue;
		}
	}
	return OUT_ANGLES;	
}
vec2 AimBot::calculateAimAngle(vec3 localO, vec3 boneO)
{
	vec3 deltaVec = { boneO.x - localO.x, boneO.y - localO.y, boneO.z - localO.z };
	float lengthDeltaVec = deltaVec.length();
	vec3 deltaVec2D;
	vec3 aimPunchAngle = memory::get().read<vec3>(GlobalVars::get().localPlayer->getBase() + GlobalVars::get().ofs.m_aimPunchAngle);
	aimPunchAngle.x *= 2.05f;
	aimPunchAngle.y *= 2.05f;
	float yaw = atan2(deltaVec.y, deltaVec.x) * (180 / PI);
	yaw -= aimPunchAngle.y;
	float pitch = -asin(deltaVec.z / lengthDeltaVec) * (180 / PI);
	pitch -= aimPunchAngle.x;
	clamp(pitch, yaw);
	return vec2(pitch, yaw);
}

void AimBot::clamp(float & pitch, float & yaw)
{
	if (pitch > 89.f || pitch < -89.f)
	{
		if (pitch > 89.f)pitch = 89.f;
		else pitch = -89.f;
	}
	if (yaw > 179.f || yaw < -179.f)
	{
		if (yaw > 179.f)yaw = -179.f;//21.06
		else yaw = 179.f;
	}
}
void AimBot::DoAimWrite()//not-safe aim cuz write process memory
{
	if (GlobalVars::get().localPlayer->health > 0)
	{
		vec3 outAngles = GetClosestTarget();
		int i = outAngles.z;
		if (GlobalVars::get().EnemyList[i] != nullptr && outAngles.x != ZEROTARGET && outAngles.y != ZEROTARGET) {
		GlobalVars::get().EnemyList[i]->bestAimTarget = true;
		}
		if (outAngles.x != ZEROTARGET && outAngles.y != ZEROTARGET && outAngles.z != ZEROTARGET) {
			int b = outAngles.z;
			if (GlobalVars::get().EnemyList[b]->health > 0) {
				vec3 viewAngles = memory::get().read<vec3>(viewAnglesAddres);
				if (GetAsyncKeyState(Config::get().aimKey))
				{
					bool isSpotted = GlobalVars::get().EnemyList[b]->spotted;
					float pitchPtr = 0;
					float yawPtr = 0;
					if (!Config::get().aimDormantCheck || (Config::get().aimDormantCheck && isSpotted))
					{
						float buffer = 180.f - abs(viewAngles.y);
						bool canFov = buffer <= Config::get().FOV / 3.f;						
						if (canFov && viewAngles.y > 0 && outAngles.y < 0)
						{
							pitchPtr = (outAngles.x - viewAngles.x) / Config::get().Smooth;
							yawPtr = (abs(outAngles.y + viewAngles.y) +1.f )/ Config::get().Smooth;
						}
						else if (canFov && viewAngles.y < 0 && outAngles.y >0)
						{
							pitchPtr = (outAngles.x - viewAngles.x) / Config::get().Smooth;
							yawPtr = ((outAngles.y + viewAngles.y)) / Config::get().Smooth - 2.f/Config::get().Smooth;
						}
						else{
							 pitchPtr = (outAngles.x - viewAngles.x) / Config::get().Smooth;
							 yawPtr = (outAngles.y - viewAngles.y) / Config::get().Smooth;
						}						
						float pitch = viewAngles.x + pitchPtr;
						float yaw = viewAngles.y + yawPtr;
						clamp(pitch, yaw);
						memory::get().write<float>(viewAnglesAddres, pitch);
						memory::get().write<float>(viewAnglesAddres + 0x4, yaw);
					}
					
				}
			}
		}
	}
}

void AimBot::doAimSafe()
{/*
	view_matrix_t m = memory::get().read<view_matrix_t>(GlobalVars::get().client + GlobalVars::get().ofs.m_ViewMatrix);
	if (GetAsyncKeyState(VK_MENU)) {
		vec3 outAngles = GetClosestTarget();
		if (outAngles.x != ZEROTARGET && outAngles.y != ZEROTARGET) {
			int b = outAngles.z;	
			POINT cp;
			GetCursorPos(&cp);
			ClientToScreen(GlobalVars::get().csgoHWND, &cp);


			float diffY = GlobalVars::get().RezY - (GlobalVars::get().marginRect.bottom - GlobalVars::get().marginRect.top);
			float distX = cp.x - GlobalVars::get().EnemyList[b]->headPos2D.x;
			float distY = cp.y - GlobalVars::get().EnemyList[b]->headPos2D.y;	

		


			if (distX > 0)
			{
				if (distY > 0)
				{
					SetCursorPos(cp.x - distX / 2.f, GlobalVars::get().EnemyList[b]->headPos2D.y);
				}
				else {
					SetCursorPos(cp.x - distX / 2.f, GlobalVars::get().EnemyList[b]->headPos2D.y);
				}				
			}
			else {
				if (distY > 0)
				{
					SetCursorPos(cp.x + abs(distX / 2.f), GlobalVars::get().EnemyList[b]->headPos2D.y);
				}
				else {
					SetCursorPos(cp.x + abs(distX / 2.f), GlobalVars::get().EnemyList[b]->headPos2D.y);
				}				
			}
		}
	}*/
}

