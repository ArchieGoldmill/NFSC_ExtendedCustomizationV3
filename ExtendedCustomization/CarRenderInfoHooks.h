#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"
#include "CarRenderInfoExtras.h"
#include "FrontEndRenderingCar.h"
#include "CarRenderConn.h"
#include "SteerAngle.h"

void __fastcall CarRenderInfoCt(CarRenderInfo* carRenderInfo, int, RideInfo* rideInfo)
{
	carRenderInfo->pRideInfo = rideInfo;
	carRenderInfo->Extras = new CarRenderInfoExtras(carRenderInfo);

	carRenderInfo->Ctor(rideInfo);
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

double __fastcall OnShadowRender(CarRenderInfo* carRenderInfo, int, bool reflection, eView* view, D3DXVECTOR4* a3, float a4, D3DXMATRIX* a5, D3DXMATRIX* a6, D3DXMATRIX* a7)
{
	carRenderInfo->Extras->IsVisible = true;

	double result = 0;
	if (reflection)
	{
		carRenderInfo->Extras->CarMatrixReflection = *carRenderInfo->Matrix;
	}
	else
	{
		carRenderInfo->Extras->CarMatrix = *carRenderInfo->Matrix;

		result = carRenderInfo->DrawAmbientShadow(view, a3, a4, a5, a6, a7);
		SAFE_CALL(carRenderInfo->Extras->Neon, RenderShadow, view, a3, a4, a5, a6, a7);
		SAFE_CALL(carRenderInfo->Extras->BrakelightGlow, RenderShadow, view, a3, a4, a5, a6, a7);
	}

	return result;
}

void __fastcall UpdateCarParts(CarRenderInfo* carRenderInfo)
{
	SAFE_CALL(carRenderInfo->Extras->Animations, FindMarkers);
	SAFE_CALL(carRenderInfo->Extras->Neon, FindMarkers);
	SAFE_CALL(carRenderInfo->Extras->WheelTextures, AdjustWheelData);

	if (!carRenderInfo->Markers.RearLicensePlate)
	{
		auto rearBumper = carRenderInfo->pRideInfo->GetPart(Slot::REAR_BUMPER);
		if (rearBumper)
		{
			carRenderInfo->Markers.RearLicensePlate = rearBumper->GetPositionMarker(Hashes::REAR_LICENSE_PLATE);
		}
	}
}

void OnAfterCarRender(CarRenderInfo* carRenderInfo, bool reflection)
{
	if (carRenderInfo)
	{
		if (carRenderInfo->Extras->IsVisible)
		{
			SAFE_CALL(carRenderInfo->Extras->Neon, RenderMarkers, reflection);
			SAFE_CALL(carRenderInfo->Extras->RotorGlow, RenderMarkers, reflection);
		}

		if (!reflection)
		{
			SAFE_CALL(carRenderInfo->Extras->Animations, Update);
			SAFE_CALL(carRenderInfo->Extras->Neon, Update);
			SAFE_CALL(carRenderInfo->Extras->ExhaustShake, Update);
			SAFE_CALL(carRenderInfo->Extras->RotorGlow, Update);

			carRenderInfo->Extras->IsVisible = false;
		}
	}
}

void PostCarRenderMain(bool reflection)
{
	if (Game::InFrontEnd())
	{
		auto rideInfo = &(FrontEndRenderingCar::Get()->RideInfo);
		OnAfterCarRender(rideInfo->pCarRenderInfo, reflection);
		SteerAngle.Update();
	}

	if (Game::InRace())
	{
		int count = CarRenderConn::GetListCount();
		auto list = CarRenderConn::GetList();
		for (int i = 0; i < count; i++)
		{
			auto carRenderInfo = list[i]->pCarRenderInfo;
			OnAfterCarRender(carRenderInfo, reflection);
		}
	}
}

void __stdcall PostCarRender()
{
	__asm pushad;

	PostCarRenderMain(false);

	__asm popad;
}

void __stdcall PostCarRenderReflection()
{
	__asm pushad;

	PostCarRenderMain(true);

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

void __declspec(naked) OnShadowRenderCave()
{
	static constexpr auto cExit = 0x007DECD2;

	__asm
	{
		push[ebp + 0x28];
		call OnShadowRender;

		jmp cExit;
	}
}

void InitCarRenderInfoHooks()
{
	injector::MakeCALL(0x007E64AE, CarRenderInfoCt);
	injector::MakeCALL(0x007E6F90, CarRenderInfoCt);
	injector::MakeJMP(0x007E640A, CarRenderInfoCtEndCave);

	injector::MakeJMP(0x007D5282, CarRenderInfoDtCave);

	injector::MakeNOP(0x007DECA7, 2);
	injector::MakeJMP(0x007DECCD, OnShadowRenderCave);

	injector::MakeJMP(0x007DA2E5, UpdateCarPartsCave);

	injector::MakeCALL(0x0072E97E, PostCarRender);
	injector::MakeCALL(0x0072E1D6, PostCarRenderReflection);
}