#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"

float __stdcall GetTireWidth(CarRenderInfo* carRenderInfo, float original, int wheel)
{
	auto rideInfo = carRenderInfo->RideInfo;
	float mult = 1.0f;
	mult += rideInfo->AutoSculptRegions[ZoneStance].Zones[wheel > 1 ? 5 : 4] / 2.0f;

	return original * mult;
}

void __declspec(naked) TireWidthCave()
{
	static constexpr auto Exit = 0x007CB8D0;

	__asm
	{
		SAVE_REGS_EAX;
		push[esp + 0x28];

		mov eax, [eax];
		push eax;

		push esi;
		call GetTireWidth;
		push eax;
		fstp[esp];
		mov eax, [esp];
		pop ebx;
		RESTORE_REGS_EAX;

		mov ebx, [esi + 0x0000324C];
		jmp Exit;
	}
}

void InitTireWidth()
{
	if (g_Config.TireWidth)
	{
		injector::MakeJMP(0x007CB8C8, TireWidthCave);
	}
}