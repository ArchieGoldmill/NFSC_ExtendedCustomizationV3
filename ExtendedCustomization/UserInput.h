#pragma once
#include <Windows.h>
#include "CarRenderInfo.h"
#include "CarRenderInfoExtras.h"
#include "FrontEndRenderingCar.h"

void ToggleAnimation(CarRenderInfo* carRenderInfo, Slot slot, int hk)
{
	if (GetAsyncKeyState(hk))
	{
		auto anim = carRenderInfo->Extras->Animations.GetAnimation(slot);
		if (anim)
		{
			anim->Toggle();
			if (slot == Slot::LEFT_HEADLIGHT)
			{
				anim = carRenderInfo->Extras->Animations.GetAnimation(Slot::RIGHT_HEADLIGHT);
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
	ToggleAnimation(carRenderInfo, Slot::HOOD, 49);
	ToggleAnimation(carRenderInfo, Slot::LEFT_HEADLIGHT, 50);
}

DWORD WINAPI HandleUserInput(LPVOID arg)
{
	while (true)
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
				auto carRenderInfo = list[i]->carRenderInfo;
				if (carRenderInfo->IsPlayer())
				{
					HandleUserInput1(carRenderInfo);
					break;
				}
			}
		}
	}

	return 0;
}

void InitUserInput()
{
	CreateThread(NULL, NULL, HandleUserInput, NULL, NULL, NULL);
}