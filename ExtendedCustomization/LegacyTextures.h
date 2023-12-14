#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"

namespace Legacy
{
	int __stdcall HeadLightsOnOff(int* renderInfo)
	{
		if (*Game::ForceHeadlightsOff)
		{
			return 1;
		}

		int* rideInfo = (int*)(*(renderInfo + 0xFC));

		if (TextureInfo::Get(*(renderInfo + 0xC20), 0, 0))
		{
			if (Game::InFrontEnd())
			{
				return 1;
			}

			if (Game::InRace())
			{
				auto info = (CarRenderInfo*)renderInfo;
				return !info->IsGlareOn(VehicleFX::LIGHT_HEADLIGHTS);
			}
		}

		return 0;
	}

	void __declspec(naked) HeadLightsOnOffCave()
	{
		static constexpr auto Exit = 0x007ADCAE;

		__asm
		{
			pushad;
			push ecx;
			call HeadLightsOnOff;
			test eax, eax;
			popad;

			je HeadLightsOff;
			mov esi, 1;
			jmp Exit;

		HeadLightsOff:
			mov esi, 0;
			jmp Exit;
		}
	}

	void InitTextures()
	{
		injector::MakeJMP(0x007ADCA8, HeadLightsOnOffCave);
	}
}