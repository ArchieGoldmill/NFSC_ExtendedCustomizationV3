#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"
#include "eLightFlare.h"
#include "eView.h"

D3DCOLOR GetSlotFlareColor(CarRenderInfo* carRenderInfo, Slot slot)
{
	auto part = carRenderInfo->pRideInfo->GetPart(slot);
	if (part)
	{
		int r = part->GetAppliedAttributeIParam(Hashes::RED, 0);
		int g = part->GetAppliedAttributeIParam(Hashes::GREEN, 0);
		int b = part->GetAppliedAttributeIParam(Hashes::BLUE, 0);

		int color = (r + (g << 8) + (b << 16));
		if (color)
		{
			return color | 0xFF000000;
		}
	}

	return 0;
}

D3DCOLOR __stdcall GetFlareColor(CarRenderInfo* carRenderInfo, eLightFlare* flare)
{
	D3DCOLOR finalColor = 0;

	Hash flareName = flare->NameHash;
	if (flareName == Hashes::RIGHT_HEADLIGHT)
	{
		finalColor = GetSlotFlareColor(carRenderInfo, Slot::RIGHT_HEADLIGHT);
	}

	if (flareName == Hashes::LEFT_HEADLIGHT)
	{
		finalColor = GetSlotFlareColor(carRenderInfo, Slot::LEFT_HEADLIGHT);
	}

	return finalColor;
}

void RenderTextureHeadlights(CarRenderInfo* carRenderInfo, eView* a1, D3DXVECTOR4* a2, float a3, D3DXMATRIX* a4, D3DXMATRIX* a5, D3DXMATRIX* a6)
{
	if (Game::InFrontEnd() && carRenderInfo->LightsState1)
	{
		// Backup
		auto backup = carRenderInfo->CarShadowTexture;
		auto min = carRenderInfo->BoundingBoxMin;
		auto max = carRenderInfo->BoundingBoxMax;

		// Draw
		carRenderInfo->BoundingBoxMax.x -= 0.3f;
		carRenderInfo->BoundingBoxMin.x = carRenderInfo->BoundingBoxMax.x + 7.0f;

		carRenderInfo->BoundingBoxMax.y *= 3;
		carRenderInfo->BoundingBoxMin.y *= 3;

		carRenderInfo->CarShadowTexture = TextureInfo::Get(Hashes::FE_HEADLIGHTS, false, false);
		carRenderInfo->DrawAmbientShadow(a1, a2, a3, a4, a5, a6);

		// Restore
		carRenderInfo->CarShadowTexture = backup;
		carRenderInfo->BoundingBoxMin = min;
		carRenderInfo->BoundingBoxMax = max;
	}
}

void RenderFrontEndFlares(CarRenderInfo* carRenderInfo, bool reflection)
{
	if (Game::InFrontEnd() && carRenderInfo->LightsState1)
	{
		if (!reflection)
		{
			FUNC(0x007260E0, void, __thiscall, sub_7260E0, int);
			sub_7260E0(0x00AAF558);

			auto matrix = &carRenderInfo->Extras->CarMatrix;
			float* position = matrix->m[3];
			carRenderInfo->RenderFlaresOnCar(eView::GetPlayer1(), position, matrix, VehicleFX_NONE, false, 0, false);
		}
	}
}

void __declspec(naked) FlareColorCave()
{
	static constexpr auto cExit = 0x007CC603;

	__asm
	{
		push 0x3F800000;
		SAVE_REGS_EAX;
		push esi;
		push[esp + 0x5C];
		call GetFlareColor;
		RESTORE_REGS_EAX;
		push eax;
		jmp cExit;
	}
}

void InitFlareColor()
{
	//injector::MakeJMP(0x007CC5FC, FlareColorCave);
}