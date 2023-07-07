#pragma once
#include "Feature.h"

void HandleSpecialCustomizationV2(FeGarageMain* feGarageMain, RideInfo* rideInfo, FECustomizationRecord* record)
{

}

int __fastcall IsSetHeadlightOn(FECarRecord* feCarRecord)
{
	auto carId = feCarRecord->GetType();

	if (g_Config.GetPopUpHeadLights(carId) == State::Enabled)
	{
		return 5;
	}

	return 0;
}

int __stdcall CarHasPopups(CarType carId)
{
	return g_Config.GetPopUpHeadLights(carId) == State::Enabled;
}

void __declspec(naked) CarHasPopupsCave()
{
	static constexpr auto cExit = 0x0085980E;
	__asm
	{
		mov eax, [esi];
		pushad;
		push eax;
		call CarHasPopups;
		cmp eax, 1;
		popad;

		jmp cExit;
	}
}

void InitLegacyCustomization()
{
	injector::MakeCALL(0x00865278, IsSetHeadlightOn, true);
	injector::MakeNOP(0x00865280, 60, true);

	injector::MakeJMP(0x00859809, CarHasPopupsCave, true);
	injector::MakeNOP(0x00859819, 23, true);
}