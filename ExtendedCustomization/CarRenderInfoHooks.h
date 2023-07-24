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
	SAFE_CALL(carRenderInfo->Extras->Neon, Init);
	SAFE_CALL(carRenderInfo->Extras->Paint, Init);
	SAFE_CALL(carRenderInfo->Extras->ExhaustShake, Init);
	SAFE_CALL(carRenderInfo->Extras->RotorGlow, Init);
	SAFE_CALL(carRenderInfo->Extras->LicensePlateText, Init);
}

void __stdcall CarRenderInfoDt(CarRenderInfo* carRenderInfo)
{
	if (carRenderInfo->Extras)
	{
		delete carRenderInfo->Extras;
		carRenderInfo->Extras = NULL;
	}
}

double __fastcall OnShadowRender(CarRenderInfo* carRenderInfo, int param, int a2, float* a3, float a4, int a5, int a6, int a7)
{
	carRenderInfo->Extras->IsVisible = true;
	carRenderInfo->Extras->CarMatrix = *carRenderInfo->GetMatrix();

	auto result = carRenderInfo->DrawAmbientShadow(a2, a3, a4, a5, a6, a7);
	SAFE_CALL(carRenderInfo->Extras->Neon, RenderShadow, a2, a3, a4, a5, a6, a7);
	SAFE_CALL(carRenderInfo->Extras->BrakelightGlow, RenderShadow, a2, a3, a4, a5, a6, a7);

	return result;
}

void __fastcall UpdateCarParts(CarRenderInfo* carRenderInfo)
{
	SAFE_CALL(carRenderInfo->Extras->Animations, FindMarkers);
	SAFE_CALL(carRenderInfo->Extras->Neon, FindMarkers);
}

void OnAfterCarRender(CarRenderInfo* carRenderInfo)
{
	if (carRenderInfo)
	{
		SAFE_CALL(carRenderInfo->Extras->Animations, Update);
		SAFE_CALL(carRenderInfo->Extras->Neon, Update);
		SAFE_CALL(carRenderInfo->Extras->ExhaustShake, Update);
		SAFE_CALL(carRenderInfo->Extras->RotorGlow, Update);

		if (carRenderInfo->Extras->IsVisible)
		{
			SAFE_CALL(carRenderInfo->Extras->Neon, RenderMarkers);
			SAFE_CALL(carRenderInfo->Extras->RotorGlow, RenderMarkers);
		}

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

		mov eax, fs: [00000000];
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
	injector::MakeJMP(0x007E55ED, CarRenderInfoCtStartCave);
	injector::MakeJMP(0x007E640A, CarRenderInfoCtEndCave);

	injector::MakeJMP(0x007D5282, CarRenderInfoDtCave);

	injector::MakeCALL(0x007DECCD, OnShadowRender);

	injector::MakeJMP(0x007DA2E5, UpdateCarPartsCave);

	injector::MakeCALL(0x0072E97E, PostCarRender);
}