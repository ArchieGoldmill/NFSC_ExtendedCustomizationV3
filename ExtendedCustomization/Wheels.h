#pragma once
#include "Feature.h"
#include "Slots.h"
#include "Constants.h"
#include "CarRenderInfo.h"
#include "CarPartDatabase.h"
#include "WheelBrands.h"
#include "CarRenderInfoExtras.h"

void ReplaceFrontWheels(CarRenderInfo* carRenderInfo, eModel* model)
{
	if (model)
	{
		model->ReplaceLightMaterial(Hashes::MAGSILVER, carRenderInfo->Materials.FrontWheel);
		model->ReplaceLightMaterial(Hashes::MAGCHROME, carRenderInfo->Materials.FrontWheel);
		model->ReplaceLightMaterial(Hashes::MAGMATTE, carRenderInfo->Materials.FrontWheel);
		model->ReplaceLightMaterial(Hashes::MAGLIP, carRenderInfo->Extras->Paint->FrontRimMaterial);
	}
}

void ReplaceRearWheels(CarRenderInfo* carRenderInfo, eModel* model)
{
	if (model)
	{
		auto rearWheel = carRenderInfo->pRideInfo->GetPart(Slot::REAR_WHEEL);
		if (rearWheel && rearWheel->GetAppliedAttributeBParam(Hashes::DEFAULT, false))
		{
			return;
		}

		model->ReplaceLightMaterial(Hashes::MAGSILVER, carRenderInfo->Extras->Paint->RearWheelMaterial);
		model->ReplaceLightMaterial(Hashes::MAGCHROME, carRenderInfo->Extras->Paint->RearWheelMaterial);
		model->ReplaceLightMaterial(Hashes::MAGMATTE, carRenderInfo->Extras->Paint->RearWheelMaterial);
		model->ReplaceLightMaterial(Hashes::MAGLIP, carRenderInfo->Extras->Paint->RearRimMaterial);
	}
}

void __cdecl SetupWheelLighting(void* light, D3DXMATRIX* transform, int m, D3DXVECTOR4* camera, eView* view, void* s)
{
	FUNC(0x00747340, int, __cdecl, SetupLighting, void* light, D3DXMATRIX * transform, int a3, D3DXVECTOR4 * camera, eView * view, void*);

	D3DXVECTOR3 scale, translation;
	D3DXQUATERNION rotation;
	D3DXMatrixDecompose(&scale, &rotation, &translation, transform);

	D3DXMATRIX matrix;
	D3DXMatrixRotationQuaternion(&matrix, &rotation);
	SetVector3(&matrix, 3, translation);

	SetupLighting(light, &matrix, m, camera, view, s);
}

void RenderWheel(CarRenderInfo* carRenderInfo, eView* view, eModel* model, D3DXMATRIX* transform, void* light, int flags, int num)
{
	if (model)
	{
		if (g_Config.CustomPaints)
		{
			IsFrontWheel(num) ? ReplaceFrontWheels(carRenderInfo, model) : ReplaceRearWheels(carRenderInfo, model);
		}

		if (carRenderInfo->Extras->WheelTextures->WheelTires[num].Tire.Model.Solid)
		{
			carRenderInfo->Extras->WheelTextures->WheelTires[num].Tire.Model.Render(view, transform, light, flags);
			model->AttachReplacementTextureTable(carRenderInfo->Extras->WheelTextures->WheelTires->Wheel.TextureTable, CarWheel::Size);
		}

		model->Render(view, transform, light, flags);
	}
}

void __fastcall RenderFrontLeftWheel(CarRenderInfo* carRenderInfo, int, eView* view, eModel** model, D3DXMATRIX* marker, void* light, int flags)
{
	RenderWheel(carRenderInfo, view, *model, marker, light, flags, WHEEL_FL);
}

void __fastcall RenderFrontRightWheel(CarRenderInfo* carRenderInfo, int, eView* view, eModel** model, D3DXMATRIX* marker, void* light, int flags)
{
	RenderWheel(carRenderInfo, view, *model, marker, light, flags, WHEEL_FR);
}

void __fastcall RenderRearRightWheel(CarRenderInfo* carRenderInfo, int, eView* view, eModel** model, D3DXMATRIX* marker, void* light, int flags)
{
	RenderWheel(carRenderInfo, view, *model, marker, light, flags, WHEEL_RR);
}

void __fastcall RenderRearLeftWheel(eView* view, int, CarRenderInfo* carRenderInfo, eModel* model, D3DXMATRIX* marker, void* light, int flags, int, int)
{
	RenderWheel(carRenderInfo, view, model, marker, light, flags, WHEEL_RL);
}

void __fastcall RenderTireSkids(void* _this, int, float a2, float* a3, D3DXMATRIX* wheelMatrix, D3DXMATRIX* carMatrix, float skidWidth)
{
	D3DXMATRIX skidMatrix = *wheelMatrix;
	float camberAdjust = 0.03;
	float skidOffset = skidWidth / 2 - camberAdjust;
	skidMatrix._42 += skidMatrix._42 < 0 ? skidOffset : -skidOffset;

	static auto RenderTireSkids = (void(__thiscall*)(void*, float, float*, D3DXMATRIX*, D3DXMATRIX*, float))0x007BFE60;
	RenderTireSkids(_this, a2, a3, &skidMatrix, carMatrix, skidWidth);
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

	if (g_Config.RemoveWheelMasking)
	{
		// Disable masking
		injector::MakeNOP(0x007B2C90, 5);
		injector::MakeNOP(0x007C358F, 5);

		// Remove rim texture replacememnts
		injector::MakeNOP(0x007D9BAA, 6);
	}

	if (g_Config.AllWheelsRotated)
	{
		// Make all wheels roated instead of mirrored on the other side of the car
		char makeWheelsRotated[3] = { 0xB0, 0x00, 0x90 };
		injector::WriteMemoryRaw(0x007E585D, makeWheelsRotated, 3);
	}

	if (g_Config.RearWheels)
	{
		// Make rear wheels installable
		injector::WriteMemory(0x007D6C76, 5);
		Game::CarPartSlotMap[(int)Slot::REAR_WHEEL] = 0x54;
		injector::MakeNOP(0x004C4818, 3);
		injector::MakeNOP(0x004C481D, 2);

		// Change rear marker to front for rear wheels
		injector::WriteMemory(0x007DF712, Hashes::FRONT_BRAKE);
		injector::WriteMemory(0x007E6057, Hashes::FRONT_BRAKE);
	}

	injector::MakeCALL(0x007DFEF7, RenderFrontLeftWheel);
	injector::MakeCALL(0x007E0098, RenderFrontRightWheel);
	injector::MakeCALL(0x007E0295, RenderRearRightWheel);
	injector::MakeJMP(0x007E0511, RenderRearLeftWheelCave);

	if (g_Config.FixWheelLighting)
	{
		injector::MakeCALL(0x007DFEC1, SetupWheelLighting);
		injector::MakeCALL(0x007E0066, SetupWheelLighting);
		injector::MakeCALL(0x007E0208, SetupWheelLighting);
		injector::MakeCALL(0x007E0464, SetupWheelLighting);
	}

	if (g_Config.FixTireSkids)
	{
		injector::MakeCALL(0x007C19CA, RenderTireSkids);
	}

	if (g_Config.FixClaiperLighting)
	{
		injector::WriteMemory<BYTE>(0x007E0C0B, 0xF);
		injector::WriteMemory<BYTE>(0x007E0D23, 0x24);
		injector::MakeNOP(0x007E0D4C, 5);
	}
}