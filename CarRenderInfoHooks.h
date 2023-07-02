#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"
#include "CarRenderInfoExtras.h"
#include "FrontEndRenderingCar.h"
#include "CarRenderConn.h"

void __stdcall CarRenderInfoCtStart(CarRenderInfo* carRenderInfo)
{
	carRenderInfo->Extras = new CarRenderInfoExtras(carRenderInfo);
}

void __stdcall CarRenderInfoCtEnd(CarRenderInfo* carRenderInfo)
{

}

void __stdcall CarRenderInfoDt(CarRenderInfo* carRenderInfo)
{
	if (carRenderInfo->Extras)
	{
		delete carRenderInfo->Extras;
		carRenderInfo->Extras = NULL;
	}
}

void __fastcall UpdateCarParts(CarRenderInfo* carRenderInfo)
{
	carRenderInfo->Extras->Animations.FindMarkers();
}

void OnAfterCarRender(CarRenderInfo* carRenderInfo)
{
	if (carRenderInfo)
	{
		carRenderInfo->Extras->Update();
		carRenderInfo->Extras->IsVisible = false;
	}
}

void __stdcall PostCarRender()
{
	__asm pushad;

	if (Game::InFrontEnd())
	{
		auto rideInfo = &(FrontEndRenderingCar::Get()->RideInfo);
		OnAfterCarRender(rideInfo->CarRenderInfo);
	}

	if (Game::InRace())
	{
		int count = CarRenderConn::GetListCount();
		auto list = CarRenderConn::GetList();
		for (int i = 0; i < count; i++)
		{
			auto carRenderInfo = list[i]->carRenderInfo;
			OnAfterCarRender(carRenderInfo);
		}
	}

	__asm popad;
}

void __declspec(naked) UpdateCarPartsCave()
{
	__asm
	{
		pop ebx;
		mov esp, ebp;
		pop ebp;

		pushad;
		call UpdateCarParts;
		popad;

		ret;
	}
}

void __declspec(naked) CarRenderInfoCtStartCave()
{
	static constexpr auto ctExit = 0x007E55F3;

	_asm
	{
		pushad;
		push ecx;
		call CarRenderInfoCtStart;
		popad;

		mov eax, fs: [00000000] ;
		jmp ctExit;
	}
}

void __declspec(naked) CarRenderInfoCtEndCave()
{
	_asm
	{
		pushad;
		push eax;
		call CarRenderInfoCtEnd;
		popad;

		ret 4;
	}
}

void __declspec(naked) CarRenderInfoDtCave()
{
	static constexpr auto dtExit = 0x007D5287;

	__asm
	{
		pushad;
		push ecx;
		call CarRenderInfoDt;
		popad;

		push 0x009AD4A3;
		jmp dtExit;
	}
}



void InitCarRenderInfoHooks()
{
	

	injector::MakeJMP(0x007E55ED, CarRenderInfoCtStartCave, true);
	injector::MakeJMP(0x007E640A, CarRenderInfoCtEndCave, true);

	injector::MakeJMP(0x007D5282, CarRenderInfoDtCave, true);

	injector::MakeJMP(0x007DA2E5, UpdateCarPartsCave, true);

	injector::MakeCALL(0x0072E97E, PostCarRender, true);
}