#include "Renderer.h"
#include <thread>
#include <chrono>
#include "LocalPlayer.h"
#include "Glow.h"
#include <string>
#include "fakelag.h"

void Renderer::drawFrames()
{
	init();
	pDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.f, 0);	//memory leak ~ 1 mb  RAM per 5 min
	pDevice->BeginScene();
	menu::get().drawMainMenu();
	Glow::get().getGlowObject();
	view_matrix_t m = memory::get().read<view_matrix_t>(GlobalVars::get().client + GlobalVars::get().ofs.m_ViewMatrix);
	if (Config::get().BoxEsp || Config::get().HpEsp || Config::get().LineEsp || Config::get().TeamEsp || Config::get().Glow || Config::get().BoxEsp || Config::get().RenderbestAimTarget || Config::get().drawFOV || Config::get().FOV_Indicate || Config::get().fakelag_indicator || Config::get().enemyWeapon) {
		for (int i = 0; i < GlobalVars::get().activeEnemyCounter; i++)
		{
			if (Config::get().Glow) {
				Glow::get().doGlow(GlobalVars::get().EnemyList[i]);
			}
			if (transformCoord(GlobalVars::get().EnemyList[i], m)) {
				//scaleCoords(GlobalVars::get().EnemyList[i]);
				if (Config::get().LineEsp)drawLine(vec2((GlobalVars::get().overlayRect.right - GlobalVars::get().overlayRect.left) / 2, GlobalVars::get().overlayRect.bottom - GlobalVars::get().overlayRect.top / 2), vec2(GlobalVars::get().EnemyList[i]->head2D.x, GlobalVars::get().EnemyList[i]->head2D.y), D3DCOLOR_XRGB(255, 0, 0));
				if (Config::get().BoxEsp)doBoxEsp(GlobalVars::get().EnemyList[i]);

				if (Config::get().RenderbestAimTarget && (Config::get().WriteAimBot || Config::get().SafeAimBot)) {
					if (GlobalVars::get().EnemyList[i]->bestAimTarget)
					{
						GlobalVars::get().EnemyList[i]->head2DByOrigin.y = GlobalVars::get().EnemyList[i]->head2DByOrigin.y;
						DrawTriangle(vec2(GlobalVars::get().EnemyList[i]->origin2D.x - GlobalVars::get().EnemyList[i]->head2D.x + GlobalVars::get().EnemyList[i]->origin2D.x, GlobalVars::get().EnemyList[i]->head2DByOrigin.y), vec2(GlobalVars::get().EnemyList[i]->width2D / 4, GlobalVars::get().EnemyList[i]->height2D / 4), D3DCOLOR_XRGB(0, 0, 0), true);
					}
				}
				if (Config::get().HpEsp)HealthEsp(GlobalVars::get().EnemyList[i]);
				if (Config::get().showHeadPos) {
				
					drawImCircle(ImVec2(GlobalVars::get().EnemyList[i]->head2D.x - 1, GlobalVars::get().EnemyList[i]->headPos2D.y - 1), 4.f - (GlobalVars::get().EnemyList[i]->width2D / GlobalVars::get().EnemyList[i]->height2D) * 0.75f, GetU32(Color(255, 0, 0,255)), 24);
				}
				if (Config::get().enemyWeapon)
				{
					DWORD nigga = memory::get().read<DWORD>(GlobalVars::get().client + GlobalVars::get().ofs.dwEntityList + ((memory::get().read<int>(GlobalVars::get().EnemyList[i]->getBase() + GlobalVars::get().ofs.m_hActiveWeapon) & 0xFFF) - 1) * 0x10);
					short index = memory::get().read<short>(nigga + GlobalVars::get().ofs.m_iItemDefinitionIndex);
					std::string name = LocalPlayer::get().get_weapon_name(index, Config::get().weaponIcons);
					int height = abs(GlobalVars::get().EnemyList[i]->headPos2D.y - GlobalVars::get().EnemyList[i]->origin2D.y);
					drawImText(Config::get().weaponIcons ?	menu::get().weaponTabs : menu::get().espFont, ImVec2(GlobalVars::get().EnemyList[i]->head2D.x, GlobalVars::get().EnemyList[i]->origin2D.y + height / 7.f), Color(255, 255, 255), name.c_str(), true, (GlobalVars::get().EnemyList[i]->distance >= 1600.f ? 10.f : 12.f));
				}
			}
			if (Config::get().FOV_Indicate)
			{
				if (!GlobalVars::get().EnemyList[i]->onFov) {
					memory::get().write<BYTE>(GlobalVars::get().EnemyList[i]->getBase() + 0x70, BYTE(255));//r
					memory::get().write<BYTE>(GlobalVars::get().EnemyList[i]->getBase() + 0x71, BYTE(255));//g
					memory::get().write<BYTE>(GlobalVars::get().EnemyList[i]->getBase() + 0x72, BYTE(255));//b
				}
				else {
					memory::get().write<BYTE>(GlobalVars::get().EnemyList[i]->getBase() + 0x70, BYTE(Config::get().fovColor[0] * 255.f));//r
					memory::get().write<BYTE>(GlobalVars::get().EnemyList[i]->getBase() + 0x71, BYTE(Config::get().fovColor[1] * 255.f));//g
					memory::get().write<BYTE>(GlobalVars::get().EnemyList[i]->getBase() + 0x72, BYTE(Config::get().fovColor[2] * 255.f));//b
				}
			}			
		}
		if (Config::get().TeamEsp)
		{
			for (int j = 0; j < GlobalVars::get().activeFriendsCounter; j++)
			{
				if (transformCoord(GlobalVars::get().FriendList[j], m)) {
					
					if (Config::get().LineEsp)drawLine(vec2((GlobalVars::get().overlayRect.right - GlobalVars::get().overlayRect.left) / 2, GlobalVars::get().overlayRect.bottom - GlobalVars::get().overlayRect.top / 2), vec2(GlobalVars::get().FriendList[j]->head2D.x, GlobalVars::get().FriendList[j]->head2D.y), D3DCOLOR_XRGB(255, 0, 0));
					if (Config::get().BoxEsp) {
						doBoxEsp(GlobalVars::get().FriendList[j]);
					}
					if (Config::get().HpEsp)HealthEsp(GlobalVars::get().FriendList[j]);
					if (Config::get().Glow)Glow::get().doGlow(GlobalVars::get().FriendList[j]);
				}
			}
		}	
	}
	if (Config::get().drawFOV)
	{
		vec2 ScreenSize = { GlobalVars::get().RezX, GlobalVars::get().RezY };
		vec2 centre = ScreenSize / 2.f;
		float ratio = ScreenSize.x / ScreenSize.y;
		float screenFov = atanf((ratio) * (0.75f) * tan((69.f * 0.5f)/180.f * 3.14f));//60 = fov of player(viewmodel_fov)
		float radiusFOV = tanf(		(Config::get().FOV /3.f	)/180.f * 3.14f	) / tanf(screenFov) * centre.x;
		if (radiusFOV >= 1 && !Config::get().FOV_Outline)
		{
			drawImCircle(ImVec2(centre.x, centre.y), radiusFOV * 0.65f, GetU32(Color(125, 50, 50, 50)), 128, true);
		}
		drawImCircle(ImVec2(centre.x, centre.y - 7), radiusFOV * 0.65f + 2,  GetU32(Color(255, 255, 255, 255)), 64, false);
	}
	if (Config::get().fakelag_indicator)
	{
			drawImRect(ImVec2(GlobalVars::get().overlayRect.right - 100, GlobalVars::get().overlayRect.top + GlobalVars::get().RezY / 2.f), ImVec2(GlobalVars::get().overlayRect.right, GlobalVars::get().overlayRect.top + GlobalVars::get().RezY / 2.f - 100), Color(255,255,255, 255), false);
			float height = 100.f;
			float chocked = (float)fakelag::get().getChockedPackets();
			float scale = 0.f; scale = chocked / 16.f;//14 = max
			if (!Config::get().fake_lag)scale = 0.f;//dumb code cuz if !fakelags=>chocked == 0 => scale == 0
			height *= scale;	
			drawImRect(ImVec2(GlobalVars::get().overlayRect.right - 100 , GlobalVars::get().overlayRect.top + GlobalVars::get().RezY / 2.f), ImVec2(GlobalVars::get().overlayRect.right, GlobalVars::get().overlayRect.top + GlobalVars::get().RezY / 2.f - height), Color(255, 0, 0, 255), true);
	}
	if (Config::get().recoilCrz)
	{

		float cX = GlobalVars::get().RezX / 2.f;
		int cY = GlobalVars::get().RezY / 2;
		float diffY = (GlobalVars::get().overlayRect.bottom - GlobalVars::get().overlayRect.top) - (GlobalVars::get().marginRect.bottom - GlobalVars::get().marginRect.top);
		vec2 crz2D = {cX, cY + diffY/6.f};
		int recoilSize = 10;
		vec2 left;
		vec2 right;
		vec2 top;
		vec2 bottom;
		left = right = top = bottom = crz2D;
		left.x -= recoilSize;
		right.x += recoilSize;
		top.y -= recoilSize;
		bottom.y += recoilSize;
		drawLine(left, right, D3DCOLOR_XRGB(255, 255, 255));
		drawLine(bottom, top, D3DCOLOR_XRGB(255, 255, 255));
	}
	menu::get().endMenuScene();
	pDevice->EndScene();
	pDevice->Present(0, 0, 0, 0);
}
void Renderer::drawLine(vec2 pos, vec2 end, D3DCOLOR color)
{
	pDevice->SetFVF(D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
	D3DCOLOR rgb = color;
	vertex vert[2] = {
		{pos.x, pos.y ,0.f, 1.0f, rgb },
		{end.x, end.y , 0.f, 1.0f, rgb}
	};
	pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, TRUE);
	pDevice->SetTexture(0, nullptr);
	pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &vert, sizeof(vertex));
	pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);
	pDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
}

void Renderer::DrawFilledRect(vec2 pos, vec2 size, D3DCOLOR color)
{
	pDevice->SetFVF(D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
	const auto toX = pos.x + size.x;
	const auto toY = pos.y + size.y;
	const auto col = color;

	vertex vert[5] =
	{
		{ pos.x, pos.y, 0.0f, 1.0f, col },
		{ toX, pos.y, 0.0f, 1.0f, col },
		{ toX, toY, 0.0f, 1.0f, col },
		{ pos.x, toY, 0.0f, 1.0f, col },
		{ pos.x, pos.y, 0.0f, 1.0f, col }
	};
	this->pDevice->SetTexture(0, nullptr);
	this->pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 3, &vert, sizeof(vertex));
}

bool Renderer::transformCoord(Player * player, view_matrix_t matrix)
{
	if (player->bestAimTarget)
	{
		player->head3DByOrigin = player->origin;
		player->head3DByOrigin.z += 90.f;
		if (!world_to_screen(vec3(GlobalVars::get().RezX, GlobalVars::get().RezY, 1.f), player->head3DByOrigin, player->head2DByOrigin, matrix))
		{
			return false;
		}
	}
	player->origin.z += 6.f;
	player->headPos3D = vec3(player->getBonePos(8, player->getBase()));	
	player->headPos3D.z += 11.f;
	if (world_to_screen(vec3(GlobalVars::get().RezX, GlobalVars::get().RezY, 1.f), player->headPos3D, player->head2D, matrix))
	{
		if (world_to_screen(vec3(GlobalVars::get().RezX, GlobalVars::get().RezY, 1.f), vec3(player->origin), player->origin2D, matrix))
		{
			if (!world_to_screen(vec3(GlobalVars::get().RezX, GlobalVars::get().RezY, 1.f), vec3(player->headPos3D.x, player->headPos3D.y, player->headPos3D.z - 11.f), player->headPos2D, matrix)) { return false; };

			player->height2D = abs(player->head2D.y - player->origin2D.y);
			player->width2D = player->height2D / 1.6f;
			player->distance = vec3::get().distance(GlobalVars::get().localPlayer->origin, player->origin);
			return true;
		}
	}
	return false;
}

void Renderer::doBoxEsp(Player* player)
{ 
	switch (Config::get().currTypeOfBox)
	{
	case 0:
	{
		float thikness = player->distance <= 1200.f ? thikness = 2.f : thikness = 1.f;
		if (Config::get().onVisible && !Config::get().isVisible)
		{
			DrawCornerBox(vec2(player->head2D.x - player->width2D / 2, player->head2D.y), vec2(player->width2D, player->height2D), !player->spotted ? D3DCOLOR_ARGB(int(Config::get().ColorBox[3] * 1000), int(Config::get().ColorBox[0] * 1000), int(Config::get().ColorBox[1] * 1000), int(Config::get().ColorBox[2] * 1000)) : D3DCOLOR_ARGB(int(Config::get().onVisibleColorBox[3] * 1000), int(Config::get().onVisibleColorBox[0] * 1000), int(Config::get().onVisibleColorBox[1] * 1000), int(Config::get().onVisibleColorBox[2] * 1000)), thikness);
			DrawCornerBox(vec2(player->head2D.x - player->width2D / 2 - 1, player->head2D.y - 1), vec2(player->width2D + 2, player->height2D + 2), D3DCOLOR_XRGB(1, 1, 1), thikness);
		}
		else if (!Config::get().onVisible && !Config::get().isVisible )
		{
			DrawCornerBox(vec2(player->head2D.x - player->width2D / 2, player->head2D.y), vec2(player->width2D, player->height2D), D3DCOLOR_ARGB(int(Config::get().ColorBox[3] * 1000),int(Config::get().ColorBox[0] * 1000), int(Config::get().ColorBox[1] * 1000), int(Config::get().ColorBox[2] * 1000)), thikness);
			DrawCornerBox(vec2(player->head2D.x - player->width2D / 2 - 1, player->head2D.y - 1), vec2(player->width2D + 2, player->height2D + 2), D3DCOLOR_XRGB(1, 1, 1), thikness);
		}
		else if (Config::get().isVisible && player->spotted) {
			DrawCornerBox(vec2(player->head2D.x - player->width2D / 2, player->head2D.y), vec2(player->width2D, player->height2D), D3DCOLOR_ARGB(int(Config::get().onVisibleColorBox[3] * 1000),int(Config::get().onVisibleColorBox[0] * 1000), int(Config::get().onVisibleColorBox[1] * 1000), int(Config::get().onVisibleColorBox[2] * 1000)), thikness);
			DrawCornerBox(vec2(player->head2D.x - player->width2D / 2 - 1, player->head2D.y - 1), vec2(player->width2D + 2, player->height2D + 2), D3DCOLOR_XRGB(1, 1, 1), thikness);
		}
		break;
	}
	case 1: {
		if (Config::get().onVisible && !Config::get().isVisible)
		{
			DrawRect(vec2(player->head2D.x - player->width2D / 2, player->head2D.y), vec2(player->width2D, player->height2D), !player->spotted ? D3DCOLOR_ARGB(int(Config::get().ColorBox[3] * 1000),int(Config::get().ColorBox[0] * 1000), int(Config::get().ColorBox[1] * 1000), int(Config::get().ColorBox[2] * 1000)) : D3DCOLOR_ARGB(int(Config::get().onVisibleColorBox[3] * 1000),int(Config::get().onVisibleColorBox[0] * 1000), int(Config::get().onVisibleColorBox[1] * 1000), int(Config::get().onVisibleColorBox[2] * 1000)));
			DrawRect(vec2(player->head2D.x - player->width2D / 2 - 1, player->head2D.y - 1), vec2(player->width2D + 2, player->height2D + 2), D3DCOLOR_XRGB(1, 1, 1));
		}
		else if (!Config::get().onVisible && !Config::get().isVisible)
		{		
			DrawRect(vec2(player->head2D.x - player->width2D/2.f - 1, player->head2D.y - 1), vec2(player->width2D + 2, player->height2D + 2), D3DCOLOR_XRGB(1, 1, 1));
			DrawRect(vec2(player->head2D.x - player->width2D/2.f, player->head2D.y), vec2(player->width2D, player->height2D), D3DCOLOR_ARGB(int(Config::get().ColorBox[3] * 1000),int(Config::get().ColorBox[0] * 1000), int(Config::get().ColorBox[1] * 1000), int(Config::get().ColorBox[2] * 1000)));
		}
		else if (Config::get().isVisible && player->spotted) {
			DrawRect(vec2(player->head2D.x - player->width2D / 2, player->head2D.y), vec2(player->width2D, player->height2D), D3DCOLOR_ARGB(int(Config::get().onVisibleColorBox[0] * 1000),int(Config::get().onVisibleColorBox[0] * 1000), int(Config::get().onVisibleColorBox[1] * 1000), int(Config::get().onVisibleColorBox[2] * 1000)));
			DrawRect(vec2(player->head2D.x - player->width2D / 2 - 1, player->head2D.y - 1), vec2(player->width2D + 2, player->height2D + 2), D3DCOLOR_XRGB(1, 1, 1));
		}
		break;
	}
	case 2: {
		if (Config::get().onVisible && !Config::get().isVisible)
		{
			DrawSecondCornerBox(vec2(player->head2D.x - player->width2D / 2, player->head2D.y), vec2(player->width2D, player->height2D), !player->spotted ? D3DCOLOR_ARGB(int(Config::get().ColorBox[3] * 1000),int(Config::get().ColorBox[0] * 1000), int(Config::get().ColorBox[1] * 1000), int(Config::get().ColorBox[2] * 1000)) : D3DCOLOR_ARGB(int(Config::get().onVisibleColorBox[3] * 1000),int(Config::get().onVisibleColorBox[0] * 1000), int(Config::get().onVisibleColorBox[1] * 1000), int(Config::get().onVisibleColorBox[2] * 1000)), 1);
		}
		else if (!Config::get().onVisible && !Config::get().isVisible)
		{
			DrawSecondCornerBox(vec2(player->head2D.x - player->width2D / 2, player->head2D.y), vec2(player->width2D, player->height2D), D3DCOLOR_ARGB(int(Config::get().ColorBox[3] * 1000),int(Config::get().ColorBox[0] * 1000), int(Config::get().ColorBox[1] * 1000), int(Config::get().ColorBox[2] * 1000)),1);
		}
		else if (Config::get().isVisible && player->spotted) {
			DrawSecondCornerBox(vec2(player->head2D.x - player->width2D / 2, player->head2D.y), vec2(player->width2D, player->height2D), D3DCOLOR_ARGB(int(Config::get().onVisibleColorBox[3] * 1000),int(Config::get().onVisibleColorBox[0] * 1000), int(Config::get().onVisibleColorBox[1] * 1000), int(Config::get().onVisibleColorBox[2] * 1000)),1);
		}
	}
	default:
		break;
	}				
}
void Renderer::scaleCoords(Player* player)
{
	//player->head2Dtransformed.y = player->head2D.y + sqrt(player->distance / (0.5f * sqrt(player->distance)));
}

void Renderer::HealthEsp(Player * player)
{
	double scale = (100.f / player->health);
	switch (Config::get().hpType)
	{
	case 2: {//left
		float height = player->height2D / scale;
		float width = 1.5f + (player->distance / sqrt(player->distance) / 25);
		if (player->distance <= 500.f)
		{
			width = 4;
		}
		if (width > 4)width = 4;
		int green = (1.f / scale * 255) * 0.7f;
		int red = (100 - player->health) * 3.2f;
		if (red > 255)red = 255;
		D3DCOLOR color = D3DCOLOR_XRGB(red, green, 0);

		DrawFilledRect(vec2(player->origin2D.x - player->width2D / 1.7f - sqrt(player->distance / (2.f * sqrt(player->distance))), player->head2D.y), vec2(width, height), color);
		DrawRect(vec2(player->origin2D.x - player->width2D / 1.7f - sqrt(player->distance / (2.f * sqrt(player->distance))), player->head2D.y), vec2(width + 1, player->height2D), D3DCOLOR_XRGB(1, 1, 1));
		break;
	}
	case 0://top
	{
		float height = 1.5f + (player->distance / sqrt(player->distance) / 25);
		if (player->distance <= 1500.f)
		{
			height = 4;
		}
		float width = player->width2D / scale;
		if (height > 3 && player->distance >= 1500.f)height = 3;
		int green = (1.f / scale * 255) * 0.7f;
		int red = (100 - player->health) * 3.2f;
		if (red > 255)red = 255;
		D3DCOLOR color = D3DCOLOR_XRGB(red, green, 0);

		DrawFilledRect(vec2(player->head2D.x - player->width2D / 2, player->head2D.y - player->head2D.y * 0.01f - 500.f / player->distance), vec2(width, height), color);
		DrawRect(vec2(player->head2D.x - player->width2D / 2, player->head2D.y - player->head2D.y * 0.01f - 500.f / player->distance), vec2(width, height), D3DCOLOR_XRGB(1, 1, 1));
		break;
	}
	case 1://bottom
	{
		float height = 1.5f + (player->distance / sqrt(player->distance) / 25);
		float width = player->width2D / scale;
		if (player->distance <= 1250.f)
		{
			height = 4.5f;
		}
		if (height > 3 && player->distance >= 1250.f)height = 3;
		int green = (1.f / scale * 255) * 0.7f;
		int red = (100 - player->health) * 3.2f;
		if (red > 255)red = 255;
		D3DCOLOR color = D3DCOLOR_XRGB(red, green, 0);

		DrawFilledRect(vec2(player->head2D.x - player->width2D / 2, player->origin2D.y + player->origin2D.y * 0.01f), vec2(width, height), color);
		DrawRect(vec2(player->head2D.x - player->width2D / 2, player->origin2D.y + player->origin2D.y * 0.01f), vec2(width, height), D3DCOLOR_XRGB(1, 1, 1));
		break;
	}
	case 3: {
		float height = player->height2D / scale;
		float width = 1.5f + (player->distance / sqrt(player->distance) / 25);
		if (player->distance <= 500.f)
		{
			width = 4;
		}
		if (width > 4)width = 4;
		int green = (1.f / scale * 255) * 0.7f;
		int red = (100 - player->health) * 3.2f;
		if (red > 255)red = 255;
		D3DCOLOR color = D3DCOLOR_XRGB(red, green, 0);

		DrawFilledRect(vec2(player->origin2D.x + player->width2D / 2.f + sqrt(player->distance / (2.f * sqrt(player->distance))), player->head2D.y), vec2(width, height), color);
		DrawRect(vec2(player->origin2D.x + player->width2D / 2.f + sqrt(player->distance / (2.f * sqrt(player->distance))), player->head2D.y), vec2(width + 1, player->height2D), D3DCOLOR_XRGB(1, 1, 1));
		break;
	}
	case 4: {
		drawImText(menu::get().defFont, ImVec2(player->head2D.x, player->head2D.y - 8.f), Color(255, 255, 255), std::to_string(player->health).c_str(),true, player->distance >= 1600.f ? 10.f : 12.f);
		break;
	}
	case 5: {
		//player->origin2D.y = player->origin2D.y + sqrt(player->distance / (2.5f * sqrt(player->distance)));//0.5f
		drawImText(menu::get().espFont, ImVec2(player->head2D.x, player->origin2D.y), Color(255, 255, 255), std::to_string(player->health).c_str(),true, player->distance >= 1600.f ? 10.f : 12.f);
		break;
	}
	case 6: {

		float height = 1.5f + (player->distance / sqrt(player->distance) / 25);
		if (player->distance <= 1500.f)
		{
			height = 4;
		}
		float width = player->width2D / scale;
		if (height > 3 && player->distance >= 1500.f)height = 3;
		int green = (1.f / scale * 255) * 0.7f;
		int red = (100 - player->health) * 3.2f;
		if (red > 255)red = 255;
		D3DCOLOR color = D3DCOLOR_XRGB(red, green, 0);
		DrawFilledRect(vec2(player->head2D.x - player->width2D / 2, player->head2D.y - player->head2D.y * 0.01f - 500.f / player->distance), vec2(width, height), color);
		DrawRect(vec2(player->head2D.x - player->width2D / 2, player->head2D.y - player->head2D.y * 0.01f - 500.f / player->distance), vec2(width, height), D3DCOLOR_XRGB(1, 1, 1));
		drawImText(menu::get().espFont, ImVec2(player->head2D.x, player->origin2D.y), Color(255, 255, 255), std::to_string(player->health).c_str(),true, player->distance >= 1600.f ? 10.f : 12.f);
		break;
		}
	}
}
void Renderer::drawImCircle(ImVec2 pos, float radius, ImU32 col, int segments, bool filled)
{
	if (!backBuffer)
	{
		backBuffer = ImGui::GetBackgroundDrawList();		
	}
	!filled ? backBuffer->AddCircle(pos, radius, col, segments, 2.f) : backBuffer->AddCircleFilled(pos, radius, col, segments);
}
void Renderer::drawImText(ImFont* font,ImVec2 pos, Color color, const char* txt, bool outline, float fontSize)
{
	if (!backBuffer)
	{
		backBuffer = ImGui::GetBackgroundDrawList();
	}
	ImVec2 textSize = font->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, txt);
	pos.x -= textSize.x / 2.f;
	if (outline)
	{
		backBuffer->AddText(font, fontSize, ImVec2(pos.x + 1, pos.y + 1), GetU32(Color(0,0,0)), txt);
		backBuffer->AddText(font, fontSize, ImVec2(pos.x - 1, pos.y -1), GetU32(Color(0, 0, 0)), txt);
		backBuffer->AddText(font, fontSize, ImVec2(pos.x + 1, pos.y - 1), GetU32(Color(0, 0, 0)), txt);
		backBuffer->AddText(font, fontSize, ImVec2(pos.x - 1, pos.y + 1), GetU32(Color(0, 0, 0)), txt);
	}
	backBuffer->AddText(font, fontSize, ImVec2(pos.x, pos.y), GetU32(color), txt);
}
void Renderer::drawImRect(ImVec2 from, ImVec2 size, Color color,bool filled)
{
	if (!backBuffer)
	{
		backBuffer = ImGui::GetBackgroundDrawList();
	}
	filled ? backBuffer->AddRectFilled(from, size, GetU32(color), 0,15) : backBuffer->AddRect(from, size, GetU32(color), 0, 15,2.f);
}
bool Renderer::world_to_screen(const vec3& screen_size, const vec3& pos, vec3& out, view_matrix_t matrix) {
	out.x = matrix[0][0] * pos.x + matrix[0][1] * pos.y + matrix[0][2] * pos.z + matrix[0][3];
	out.y = matrix[1][0] * pos.x + matrix[1][1] * pos.y + matrix[1][2] * pos.z + matrix[1][3];

	float w = matrix[3][0] * pos.x + matrix[3][1] * pos.y + matrix[3][2] * pos.z + matrix[3][3];

	if (w < 0.001f)
		return false;

	float inv_w = 1.f / w;
	out.x *= inv_w;
	out.y *= inv_w;

	float x = screen_size.x * .5f;
	float y = screen_size.y * .5f;

	x += 0.5f * out.x * screen_size.x + 0.5f;
	y -= 0.5f * out.y * screen_size.y + 0.5f;

	out.x = x;
	out.y = y;

	return true;
}
void Renderer::DrawRect(vec2 pos, vec2 size, D3DCOLOR color)
{
	pDevice->SetFVF(D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
	const auto toX = pos.x + size.x;
	const auto toY = pos.y + size.y;
	const auto col = color;

	vertex vert[5] =
	{
		{ pos.x, pos.y, 0.0f, 1.0f, col },
		{ toX, pos.y, 0.0f, 1.0f, col },
		{ toX, toY, 0.0f, 1.0f, col },
		{ pos.x, toY, 0.0f, 1.0f, col },
		{ pos.x, pos.y, 0.0f, 1.0f, col }
	};
	this->pDevice->SetTexture(0, nullptr);
	this->pDevice->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, &vert, sizeof(vertex));
}

void Renderer::DrawCornerBox(vec2 pos, vec2 size, D3DCOLOR color, float thickness)
{
	DrawRect(pos, vec2(size.x / 3.f, thickness), color);
	DrawRect(vec2(pos.x + size.x, pos.y), vec2(-size.x / 3.f, thickness), color);//;top

	DrawRect(pos, vec2(thickness, size.y / 3.f), color);
	DrawRect(vec2(pos.x, pos.y + size.y), vec2(thickness, -size.y / 3.f), color);//left

	DrawRect(vec2(pos.x, pos.y + size.y), vec2(size.x / 3.f, thickness), color);
	DrawRect(vec2(pos.x + size.x, pos.y + size.y), vec2(-size.x / 3.f, thickness), color);//bottom


	DrawRect(vec2(pos.x + size.x, pos.y), vec2(thickness, size.y / 3.f), color);
	DrawRect(vec2(pos.x + size.x, pos.y + size.y), vec2(thickness, -size.y / 3.f), color);//right
}

void Renderer::DrawSecondCornerBox(vec2 pos, vec2 size, D3DCOLOR color, float thikness)
{
	drawLine(pos, vec2(pos.x + size.x / 3, pos.y), color);//top left
	drawLine(vec2(pos.x + size.x, pos.y), vec2(pos.x  +size.x - size.x/3.f, pos.y), color);//top right


	drawLine(vec2(pos.x, pos.y), vec2(pos.x, pos.y + size.y), color);//left
	drawLine(vec2(pos.x + size.x, pos.y), vec2(pos.x + size.x, pos.y + size.y), color);//rgijt


	drawLine(vec2(pos.x , pos.y + size.y), vec2(pos.x + size.x / 3, pos.y + size.y), color);//bottom left
	drawLine(vec2(pos.x + size.x, pos.y + size.y), vec2(pos.x + size.x - size.x/3.f, pos.y + size.y), color);//bottom right
}

void Renderer::DrawTriangle(vec2 pos, vec2 size, D3DCOLOR color, bool rainbow)
{
	pDevice->SetFVF(D3DFVF_DIFFUSE | D3DFVF_XYZRHW);
	const auto toX = pos.x + size.x;
	const auto toY = pos.y + size.y;
	const auto col = color;
	vertex vert[3] = { 0 };
	if (rainbow)
	{
		vertex vertBUFF[3] = {
		{pos.x, pos.y, 0.0f, 1.0f, D3DCOLOR_XRGB(255,0,0) },
		{toX, pos.y, 0.0f, 1.0f, D3DCOLOR_XRGB(0,255,0) },
		{pos.x + size.x / 2, toY, 0.0f, 1.0f, D3DCOLOR_XRGB(0,0,255) },
		//{pos.x, pos.y, 0.0f, 1.0f, col },
		};
		vert[0] = vertBUFF[0];
		vert[1] = vertBUFF[1];
		vert[2] = vertBUFF[2];
	}
	else {
		vertex vertBUFF[3] = {
		{pos.x, pos.y, 0.0f, 1.0f, D3DCOLOR_XRGB(255,0,0) },
		{toX, pos.y, 0.0f, 1.0f, D3DCOLOR_XRGB(0,255,0) },
		{pos.x + size.x / 2, toY, 0.0f, 1.0f, D3DCOLOR_XRGB(0,0,255) },
		//{pos.x, pos.y, 0.0f, 1.0f, col },
		};
		vert[0] = vertBUFF[0];
		vert[1] = vertBUFF[1];
		vert[2] = vertBUFF[2];
	}
	this->pDevice->SetTexture(0, nullptr);
	this->pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 1, &vert, sizeof(vertex));
}

void Renderer::drawString(vec2 pos, vec2 size, const char * name)
{
	if (GlobalVars::get().defFont)
	{
		pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		RECT rc = { pos.x, pos.y, pos.x + size.x, pos.y + size.y };
		GlobalVars::get().defFont->DrawTextA(0, name, strlen(name), &rc, DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
	}
}

Color::Color()
{
	*((int *)this) = 0;
}
Color::Color(int _r, int _g, int _b)
{
	SetColor(_r, _g, _b, 255);
}
Color::Color(int _r, int _g, int _b, int _a)
{
	SetColor(_r, _g, _b, _a);
}
void Color::SetRawColor(int color32)
{
	*((int *)this) = color32;
}
int Color::GetRawColor() const
{
	return *((int *)this);
}
void Color::SetColor(int _r, int _g, int _b, int _a)
{
	_CColor[0] = (unsigned char)_r;
	_CColor[1] = (unsigned char)_g;
	_CColor[2] = (unsigned char)_b;
	_CColor[3] = (unsigned char)_a;
}
void Color::SetColor(float _r, float _g, float _b, float _a)
{
	_CColor[0] = static_cast<unsigned char>(_r * 255.0f);
	_CColor[1] = static_cast<unsigned char>(_g * 255.0f);
	_CColor[2] = static_cast<unsigned char>(_b * 255.0f);
	_CColor[3] = static_cast<unsigned char>(_a * 255.0f);
}
void Color::GetColor(int &_r, int &_g, int &_b, int &_a) const
{
	_r = _CColor[0];
	_g = _CColor[1];
	_b = _CColor[2];
	_a = _CColor[3];
}
bool Color::operator== (const Color &rhs) const
{
	return (*((int *)this) == *((int *)&rhs));
}
bool Color::operator!= (const Color &rhs) const
{
	return !(operator==(rhs));
}
Color& Color::operator=(const Color &rhs)
{
	SetRawColor(rhs.GetRawColor());
	return *this;
}
