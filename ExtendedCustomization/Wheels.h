#pragma once
#include "Feature.h"
#include "Slots.h"
#include "Constants.h"
#include "CarRenderInfo.h"
#include "CarPartDatabase.h"
#include "WheelBrands.h"
#include "CarRenderInfoExtras.h"

#define WHEEL_FL 0
#define WHEEL_FR 1
#define WHEEL_RR 2
#define WHEEL_RL 3

void ReplaceFrontWheels(CarRenderInfo* carRenderInfo, eModel* model)
{
	if (model)
	{
		model->ReplaceLightMaterial(Hashes::MAGSILVER, carRenderInfo->Materials.FrontRims);
		model->ReplaceLightMaterial(Hashes::MAGLIP, carRenderInfo->Extras->Paint->FrontLipMaterial);
		carRenderInfo->Extras->Paint->FrontWheel = model;
	}
}

void ReplaceRearWheels(CarRenderInfo* carRenderInfo, eModel* model)
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

void RenderWheel(CarRenderInfo* carRenderInfo, int view, eModel** model, D3DXMATRIX* marker, D3DXMATRIX* light, int data, int wheel)
{
	if (model && *model)
	{
		if (g_Config.CustomPaints)
		{
			wheel < 2 ? ReplaceFrontWheels(carRenderInfo, *model) : ReplaceRearWheels(carRenderInfo, *model);
		}

		(*model)->Render(view, marker, light, data);
	}
}

void __fastcall RenderFrontLeftWheel(CarRenderInfo* carRenderInfo, int, int view, eModel** model, D3DXMATRIX* marker, D3DXMATRIX* light, int data)
{
	RenderWheel(carRenderInfo, view, model, marker, light, data, WHEEL_FL);
}

void __fastcall RenderFrontRightWheel(CarRenderInfo* carRenderInfo, int, int view, eModel** model, D3DXMATRIX* marker, D3DXMATRIX* light, int data)
{
	RenderWheel(carRenderInfo, view, model, marker, light, data, WHEEL_FR);
}

void __fastcall RenderRearRightWheel(CarRenderInfo* carRenderInfo, int, int view, eModel** model, D3DXMATRIX* marker, D3DXMATRIX* light, int data)
{
	RenderWheel(carRenderInfo, view, model, marker, light, data, WHEEL_RR);
}

void __fastcall RenderRearLeftWheel(int view, int, CarRenderInfo* carRenderInfo, eModel* model, D3DXMATRIX* marker, D3DXMATRIX* light, int data, int a1, int a2)
{
	RenderWheel(carRenderInfo, view, &model, marker, light, data, WHEEL_RL);
}

void __declspec(naked) RenderRearLeftWheelCave()
{
	static constexpr auto cExit = 0x007E0516;

	__asm
	{
		push esi;
		call RenderRearLeftWheel;
		jmp cExit;
	}
}

void InitWheels()
{
	InitWheelBrands();

	// Disable masking
	injector::MakeNOP(0x007B2C90, 5);
	injector::MakeNOP(0x007C358F, 5);

	// Remove rim texture replacememnts
	injector::MakeNOP(0x007D9BAA, 6);

	// Make all wheels roated instead of mirrored on the other side of the car
	char makeWheelsRotated[3] = { 0xB0, 0x00, 0x90 };
	injector::WriteMemoryRaw(0x007E585D, makeWheelsRotated, 3);

	// Make rear wheels installable
	injector::WriteMemory(0x007D6C76, 5, true);
	Game::CarPartSlotMap[(int)Slot::REAR_WHEEL] = 0x54;

	injector::MakeCALL(0x007DFEF7, RenderFrontLeftWheel);
	injector::MakeCALL(0x007E0098, RenderFrontRightWheel);
	injector::MakeCALL(0x007E0295, RenderRearRightWheel);
	injector::MakeJMP(0x007E0511, RenderRearLeftWheelCave);

	// Change rear marker to front for rear wheels
	injector::WriteMemory(0x007DF712, Hashes::FRONT_BRAKE);
	injector::WriteMemory(0x007E6057, Hashes::FRONT_BRAKE);
}