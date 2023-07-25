#pragma once
#include "Feature.h"
#include "Slots.h"
#include "Constants.h"
#include "CarRenderInfo.h"

#define WHEEL_FL 0
#define WHEEL_FR 1
#define WHEEL_RR 2
#define WHEEL_RL 3

void RenderWheel(CarRenderInfo* carRenderInfo, int view, eModel** model, D3DXMATRIX* marker, D3DXMATRIX* light, int data, int wheel)
{
	if (model && *model)
	{
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

void InitRims()
{
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

	Game::AutosculptRegionList[ZoneRearWheel] = Slot::REAR_WHEEL;

	injector::MakeCALL(0x007DFEF7, RenderFrontLeftWheel);
	injector::MakeCALL(0x007E0098, RenderFrontRightWheel);
	injector::MakeCALL(0x007E0295, RenderRearRightWheel);
	injector::MakeJMP(0x007E0511, RenderRearLeftWheelCave);
}