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
		model->ReplaceLightMaterial(Hashes::MAGMATTE, carRenderInfo->Materials.FrontRims);
		model->ReplaceLightMaterial(Hashes::MAGLIP, carRenderInfo->Extras->Paint->FrontLipMaterial);
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

		model->ReplaceLightMaterial(Hashes::MAGSILVER, carRenderInfo->Extras->Paint->RearRimMaterial);
		model->ReplaceLightMaterial(Hashes::MAGCHROME, carRenderInfo->Extras->Paint->RearRimMaterial);
		model->ReplaceLightMaterial(Hashes::MAGMATTE, carRenderInfo->Extras->Paint->RearRimMaterial);
		model->ReplaceLightMaterial(Hashes::MAGLIP, carRenderInfo->Extras->Paint->RearLipMaterial);
	}
}

void AdjustWheelLighting(eView* view, D3DXMATRIX* transform, void* light, void* s)
{
	FUNC(0x00747340, int, __cdecl, SetupLighting, void* light, D3DXMATRIX * transform, int a3, D3DXVECTOR4 * camera, eView * view, void*);

	D3DXVECTOR3 scale, translation;
	D3DXQUATERNION rotation;
	D3DXMatrixDecompose(&scale, &rotation, &translation, transform);

	D3DXMATRIX matrix;
	D3DXMatrixRotationQuaternion(&matrix, &rotation);
	SetVector3(&matrix, 3, translation);

	D3DXVECTOR4 camera = view->pCamera->CurrentKey.Position;
	camera.w = 1;
	SetupLighting(light, &matrix, 0x00B1F5F0, &camera, view, s);
}

void RenderWheel(CarRenderInfo* carRenderInfo, eView* view, eModel** model, D3DXMATRIX* marker, void* light, int flags, void* s, int wheel)
{
	if (model && *model)
	{
		AdjustWheelLighting(view, marker, light, s);

		if (g_Config.CustomPaints)
		{
			wheel < 2 ? ReplaceFrontWheels(carRenderInfo, *model) : ReplaceRearWheels(carRenderInfo, *model);
		}

		int side = wheel < 2 ? 0 : 1;
		carRenderInfo->Extras->WheelTextures->Tires[side].Render(view, marker, light, flags);
		(*model)->AttachReplacementTextureTable(carRenderInfo->Extras->WheelTextures->TextureTable[side].TextureTable, 2);

		(*model)->Render(view, marker, light, flags);
	}
}

void __fastcall RenderFrontLeftWheel(CarRenderInfo* carRenderInfo, int, void* s, eView* view, eModel** model, D3DXMATRIX* marker, void* light, int flags)
{
	RenderWheel(carRenderInfo, view, model, marker, light, flags, s, WHEEL_FL);
}

void __fastcall RenderFrontRightWheel(CarRenderInfo* carRenderInfo, int, void* s, eView* view, eModel** model, D3DXMATRIX* marker, void* light, int flags)
{
	RenderWheel(carRenderInfo, view, model, marker, light, flags, s, WHEEL_FR);
}

void __fastcall RenderRearRightWheel(CarRenderInfo* carRenderInfo, int, void* s, eView* view, eModel** model, D3DXMATRIX* marker, void* light, int flags)
{
	RenderWheel(carRenderInfo, view, model, marker, light, flags, s, WHEEL_RR);
}

void __fastcall RenderRearLeftWheel(eView* view, int, void* s, CarRenderInfo* carRenderInfo, eModel* model, D3DXMATRIX* marker, void* light, int flags, int, int)
{
	RenderWheel(carRenderInfo, view, &model, marker, light, flags, s, WHEEL_RL);
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

void __declspec(naked) RenderFrontLeftWheelCave()
{
	static constexpr auto cExit = 0x007DFEFC;

	__asm
	{
		lea eax, [esp + 0x5A4];
		push eax;
		call RenderFrontLeftWheel;
		jmp cExit;
	}
}

void __declspec(naked) RenderFrontRightWheelCave()
{
	static constexpr auto cExit = 0x007E009D;

	__asm
	{
		lea eax, [esp + 0x5A4];
		push eax;
		call RenderFrontRightWheel;
		jmp cExit;
	}
}

void __declspec(naked) RenderRearRightWheelCave()
{
	static constexpr auto cExit = 0x007E029A;

	__asm
	{
		lea eax, [esp + 0x5A4];
		push eax;
		call RenderRearRightWheel;
		jmp cExit;
	}
}

void __declspec(naked) RenderRearLeftWheelCave()
{
	static constexpr auto cExit = 0x007E0516;

	__asm
	{
		push esi;
		lea eax, [esp + 0x5AC];
		push eax;
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

	injector::MakeJMP(0x007DFEF7, RenderFrontLeftWheelCave);
	injector::MakeJMP(0x007E0098, RenderFrontRightWheelCave);
	injector::MakeJMP(0x007E0295, RenderRearRightWheelCave);
	injector::MakeJMP(0x007E0511, RenderRearLeftWheelCave);

	if (g_Config.FixTireSkids)
	{
		injector::MakeCALL(0x007C19CA, RenderTireSkids);
	}

	if (g_Config.FixClaiperLighting)
	{
		// Save car's lighting context
		BYTE brakeLightFix[] = { 0x8B, 0x44, 0x24, 0x2C, 0x89, 0x84, 0x24, 0xF4, 0x00, 0x00, 0x00 };
		injector::WriteMemoryRaw(0x007DE6EB, brakeLightFix, sizeof(brakeLightFix));

		// Pass lighting context to brakes, so they do not flicker, since they are not rotating with the wheel
		injector::WriteMemory<DWORD>(0x007E0C2C, 0x9090C931);
		injector::WriteMemory<DWORD64>(0x007E0C34, 0x548B000000F8C181);
		injector::WriteMemory<DWORD64>(0x007E0C43, 0x084D8B9031FFE101);
	}
}