#pragma once
#include <Windows.h>
#include "CarRenderInfo.h"
#include "CarRenderInfoExtras.h"
#include "FrontEndRenderingCar.h"

void ToggleAnimation(CarRenderInfo* carRenderInfo, Slot slot, int hk)
{
	if (GetAsyncKeyState(hk))
	{
		auto anim = (PartAnimation*)carRenderInfo->Extras->Animations->GetAnimation(slot);
		if (anim)
		{
			anim->Toggle();
			if (slot == Slot::LEFT_HEADLIGHT)
			{
				anim = (PartAnimation*)carRenderInfo->Extras->Animations->GetAnimation(Slot::RIGHT_HEADLIGHT);
				if (anim)
				{
					anim->Toggle();
				}
			}
		}

		while ((GetAsyncKeyState(hk) & 0x8000) > 0) { Sleep(0); }
	}
}

void HandleUserInput1(CarRenderInfo* carRenderInfo)
{
	ToggleAnimation(carRenderInfo, Slot::HOOD, g_Config.HK_ToggleHood);
	ToggleAnimation(carRenderInfo, Slot::LEFT_HEADLIGHT, g_Config.HK_ToggleHeadlights);
	ToggleAnimation(carRenderInfo, Slot::DOOR_LEFT, g_Config.HK_ToggleLeftDoor);
	ToggleAnimation(carRenderInfo, Slot::DOOR_RIGHT, g_Config.HK_ToggleRightDoor);
	ToggleAnimation(carRenderInfo, Slot_Trunk, g_Config.HK_ToggleTrunk);
}

DWORD WINAPI HandleUserInput(LPVOID arg)
{
	while (true)
	{
		if (*Game::FePhotoModeMenuScreen_Instance)
		{
			if (Game::InFrontEnd())
			{
				auto rideInfo = &(FrontEndRenderingCar::Get()->RideInfo);
				HandleUserInput1(rideInfo->CarRenderInfo);
			}

			if (Game::InRace())
			{
				int count = CarRenderConn::GetListCount();
				auto list = CarRenderConn::GetList();
				for (int i = 0; i < count; i++)
				{
					auto carRenderInfo = list[i]->pCarRenderInfo;
					if (carRenderInfo->IsPlayer())
					{
						HandleUserInput1(carRenderInfo);
						break;
					}
				}
			}
		}
	}

	return 0;
}

void InitUserInput()
{
	if (g_Config.HK_Enabled)
	{
		CreateThread(NULL, NULL, HandleUserInput, NULL, NULL, NULL);
	}
}