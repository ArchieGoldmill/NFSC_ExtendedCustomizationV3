#pragma once
#include <Windows.h>
#include "CarRenderInfo.h"
#include "CarRenderInfoExtras.h"
#include "FERenderingCar.h"
#include "../ThirdParty/IniReader/IniReader.h"

bool CarRenderInfo::IsLightOn()
{
	return this->Extras->IsLightOn;
}

bool CarRenderInfo::IsEngineOn()
{
	return this->Extras->IsEngineOn;
}

void ToggleAnimation(CarRenderInfo* carRenderInfo, Slot slot, int hk)
{
	if (GetAsyncKeyState(hk))
	{
		if (*Game::FePhotoModeMenuScreen_Instance && !*Game::NotInFocus)
		{
			auto anim = (PartAnimation*)carRenderInfo->Extras->Animations->GetPartAnimation(slot);
			if (anim)
			{
				anim->Toggle();
				if (slot == Slot::LEFT_HEADLIGHT)
				{
					anim = (PartAnimation*)carRenderInfo->Extras->Animations->GetPartAnimation(Slot::RIGHT_HEADLIGHT);
					if (anim)
					{
						anim->Toggle();
					}
				}
			}

			while ((GetAsyncKeyState(hk) & 0x8000) > 0) { Sleep(0); }
		}
	}
}

void HandleUserInput1(CarRenderInfo* carRenderInfo)
{
	ToggleAnimation(carRenderInfo, Slot::HOOD, g_Config.HK_ToggleHood);
	ToggleAnimation(carRenderInfo, Slot::LEFT_HEADLIGHT, g_Config.HK_ToggleHeadlights);
	ToggleAnimation(carRenderInfo, Slot::DOOR_LEFT, g_Config.HK_ToggleLeftDoor);
	ToggleAnimation(carRenderInfo, Slot::DOOR_RIGHT, g_Config.HK_ToggleRightDoor);
	ToggleAnimation(carRenderInfo, Slot_Trunk, g_Config.HK_ToggleTrunk);

	if (GetAsyncKeyState(g_Config.HK_ToggleLights))
	{
		if (!*Game::NotInFocus)
		{
			carRenderInfo->Extras->IsLightOn = !carRenderInfo->Extras->IsLightOn;
			if (Game::InFrontEnd())
			{
				if (carRenderInfo->Extras->IsLightOn)
				{
					carRenderInfo->LightsState1 = VehicleFX_LIGHTS;
				}
				else
				{
					carRenderInfo->LightsState1 = VehicleFX_NONE;
				}
			}

			auto anim = (PartAnimation*)carRenderInfo->Extras->Animations->GetPartAnimation(Slot::LEFT_HEADLIGHT);
			if (anim)
			{
				float target = carRenderInfo->LightsState1 ? 1.0f : 0.0f;
				anim->SetTarget(target);

				anim = (PartAnimation*)carRenderInfo->Extras->Animations->GetPartAnimation(Slot::RIGHT_HEADLIGHT);
				if (anim)
				{
					anim->SetTarget(target);
				}
			}
		}

		while ((GetAsyncKeyState(g_Config.HK_ToggleLights) & 0x8000) > 0) { Sleep(0); }
	}

	if (GetAsyncKeyState(g_Config.HK_ToggleEngine))
	{
		if (!*Game::NotInFocus)
		{
			carRenderInfo->Extras->IsEngineOn = !carRenderInfo->Extras->IsEngineOn;
		}

		while ((GetAsyncKeyState(g_Config.HK_ToggleLights) & 0x8000) > 0) { Sleep(0); }
	}
}

DWORD WINAPI HandleUserInput(LPVOID arg)
{
	while (true)
	{
		if (Game::InFrontEnd())
		{
			HandleUserInput1(FERenderingCar::GetRideInfo()->pCarRenderInfo);
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

		Sleep(100);
	}

	return 0;
}

void InitUserInput()
{
	if (g_Config.HK_Enabled)
	{
		CreateThread(NULL, NULL, HandleUserInput, NULL, NULL, NULL);

		CIniReader extraOptions("NFSCExtraOptionsSettings.ini");
		if (extraOptions.ReadInteger("Hotkeys", "Headlights", 0))
		{
			extraOptions.WriteInteger("Hotkeys", "Headlights", 0);
		}
	}
}