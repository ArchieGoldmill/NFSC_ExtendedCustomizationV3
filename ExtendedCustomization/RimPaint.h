#pragma once
#include "CarRenderInfo.h"
#include "CarRenderInfoExtras.h"

void __stdcall ReplaceFrontWheels(CarRenderInfo* carRenderInfo, eModel* model)
{
	if (model)
	{
		model->ReplaceLightMaterial(Hashes::MAGSILVER, carRenderInfo->Materials.FrontRims);
		model->ReplaceLightMaterial(Hashes::MAGLIP, carRenderInfo->Extras->Paint->FrontLipMaterial);
		carRenderInfo->Extras->Paint->FrontWheel = model;
	}
}

void __stdcall ReplaceRearWheels(CarRenderInfo* carRenderInfo, eModel* model)
{
	if (model)
	{
		auto frontWheel = carRenderInfo->Extras->Paint->FrontWheel;
		if (frontWheel && frontWheel->Solid == model->Solid)
		{
			// Skip material replace if we dont have mesh for rear wheels
			return;
		}

		model->ReplaceLightMaterial(Hashes::MAGSILVER, carRenderInfo->Extras->Paint->RearRimMaterial);
		model->ReplaceLightMaterial(Hashes::MAGLIP, carRenderInfo->Extras->Paint->RearLipMaterial);
	}
}

void __declspec(naked) ReplaceFrontWheelsCave()
{
	static constexpr auto cExit = 0x007DFC06;
	__asm
	{
		pushad;
		mov edi, [edi];
		push edi;
		push esi;
		call ReplaceFrontWheels;
		popad;
		jmp cExit;
	}
}

void __declspec(naked) ReplaceRearWheelsCave()
{
	static constexpr auto cExit = 0x007DFC4A;
	__asm
	{
		pushad;
		mov eax, [eax];
		push eax;
		push esi;
		call ReplaceRearWheels;
		popad;
		jmp cExit;
	}
}

void InitRimPaint()
{
	injector::MakeJMP(0x007DFBC4, ReplaceFrontWheelsCave);
	injector::MakeJMP(0x007DFC06, ReplaceRearWheelsCave);
}