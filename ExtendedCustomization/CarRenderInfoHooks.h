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
	if (carRenderInfo->Extras->Neon)
	{
		carRenderInfo->Extras->Neon->Init();
	}

	carRenderInfo->Extras->Paint.Init();

	if (carRenderInfo->Extras->ExhaustShake)
	{
		carRenderInfo->Extras->ExhaustShake->Init();
	}

	carRenderInfo->Extras->Textures.Init();

	if (carRenderInfo->Extras->RotorGlow)
	{
		carRenderInfo->Extras->RotorGlow->Init();
	}
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
	if (carRenderInfo->Extras->Neon)
	{
		carRenderInfo->Extras->Neon->RenderShadow(a2, a3, a4, a5, a6, a7);
	}

	if (carRenderInfo->Extras->BrakelightGlow)
	{
		carRenderInfo->Extras->BrakelightGlow->Render(a2, a3, a4, a5, a6, a7);
	}

	return result;
}

void __fastcall UpdateCarParts(CarRenderInfo* carRenderInfo)
{
	carRenderInfo->Extras->Animations.FindMarkers();
	if (carRenderInfo->Extras->Neon)
	{
		carRenderInfo->Extras->Neon->FindMarkers();
	}
}

void OnAfterCarRender(CarRenderInfo* carRenderInfo)
{
	if (carRenderInfo)
	{
		carRenderInfo->Extras->Animations.Update();
		if (carRenderInfo->Extras->Neon)
		{
			carRenderInfo->Extras->Neon->Update();
		}

		if (carRenderInfo->Extras->ExhaustShake)
		{
			carRenderInfo->Extras->ExhaustShake->Update();
		}

		if (carRenderInfo->Extras->RotorGlow)
		{
			carRenderInfo->Extras->RotorGlow->Update();
		}

		if (carRenderInfo->Extras->IsVisible)
		{
			if (carRenderInfo->Extras->Neon)
			{
				carRenderInfo->Extras->Neon->RenderMarkers();
			}

			if (carRenderInfo->Extras->RotorGlow)
			{
				carRenderInfo->Extras->RotorGlow->Render();
			}
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