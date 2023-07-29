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
		model->ReplaceLightMaterial(Hashes::MAGCHROME, carRenderInfo->Materials.FrontRims);
		model->ReplaceLightMaterial(Hashes::MAGLIP, carRenderInfo->Extras->Paint->FrontLipMaterial);
	}
}

void ReplaceRearWheels(CarRenderInfo* carRenderInfo, eModel* model)
{
	if (model)
	{
		auto rearWheel = carRenderInfo->RideInfo->GetPart(Slot::REAR_WHEEL);
		if (rearWheel && rearWheel->GetAppliedAttributeBParam(Hashes::DEFAULT, false))
		{
			return;
		}

		model->ReplaceLightMaterial(Hashes::MAGSILVER, carRenderInfo->Extras->Paint->RearRimMaterial);
		model->ReplaceLightMaterial(Hashes::MAGCHROME, carRenderInfo->Extras->Paint->RearRimMaterial);
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

		int side = wheel < 2 ? 0 : 1;
		carRenderInfo->Extras->WheelTextures->Tires[side].Render(view, marker, light, data);
		(*model)->AttachReplacementTextureTable(carRenderInfo->Extras->WheelTextures->TextureTable[side].TextureTable, 4);

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

	if (g_Config.FixClaiperLighting)
	{
		// Save car's lighting context
		BYTE brakeLightFix[] = { 0x8B, 0x44, 0x24, 0x2C, 0x89, 0x84, 0x24, 0xF4, 0x00, 0x00, 0x00 };
		injector::WriteMemoryRaw(0x007DE6EB, brakeLightFix, sizeof(brakeLightFix), true);

		// Pass lighting context to brakes, so they do not flicker, since they are not rotating with the wheel
		injector::WriteMemory<DWORD>(0x007E0C2C, 0x9090C931, true);
		injector::WriteMemory<DWORD64>(0x007E0C34, 0x548B000000F8C181, true);
		injector::WriteMemory<DWORD64>(0x007E0C43, 0x084D8B9031FFE101, true);
	}
}