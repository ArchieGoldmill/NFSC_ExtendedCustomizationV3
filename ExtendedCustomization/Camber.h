#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"
#include "Constants.h"

float __stdcall GetCamber(CarRenderInfo* carRenderInfo, int zone)
{
	return carRenderInfo->RideInfo->AutoSculptRegions[ZoneStance].Zones[zone];
}

void __declspec(naked) RearCamberCave()
{
	static constexpr auto cExit = 0x007DF7B4;

	__asm
	{
		pushad;
		push 1;
		push esi;
		call GetCamber;
		push eax;
		fstp[esp];
		fadd[esp];
		pop eax;
		popad;
		jmp cExit;
	}
}

void __declspec(naked) FrontCamberCave()
{
	static constexpr auto cExit = 0x007DF78E;

	__asm
	{
		pushad;
		push 0;
		push esi;
		call GetCamber;
		push eax;
		fstp[esp];
		fadd[esp];
		pop eax;
		popad;
		jmp cExit;
	}
}

float CamberMult = 0;
void InitCamber()
{
	if (g_Config.Camber)
	{
		injector::WriteMemory<float*>(0x007DF76D, &CamberMult);
		injector::MakeJMP(0x007DF788, FrontCamberCave);
		injector::MakeJMP(0x007DF7AE, RearCamberCave);

		// Allow camber for lods
		injector::MakeNOP(0x007DF77E, 2);
	}
}