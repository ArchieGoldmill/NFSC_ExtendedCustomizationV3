#pragma once
#include "Func.h"
#include "Feature.h"
#include "Config.h"

void* __fastcall FeVinylTransformCt(void* _this, int, void* menuScreen)
{
	*Game::FeFastRep = -295;
	FUNC(0x0085C100, void*, __thiscall, _FeVinylTransformCt, void*, void*);
	return _FeVinylTransformCt(_this, menuScreen);
}

void __fastcall FeVinylTransformDt(void* _this)
{
	*Game::FeFastRep = -200;
	FUNC(0x00841330, void*, __thiscall, _FeVinylTransformDt, void*);
	_FeVinylTransformDt(_this);
}

void InitSmoothVinyls()
{
	if (g_Config.SmoothVinylTransform)
	{
		injector::WriteMemory(Game::FeFastRep, -200, true);
		injector::MakeCALL(0x00574F96, FeVinylTransformCt, true);
		injector::MakeCALL(0x00851BF3, FeVinylTransformDt, true);
	}
}