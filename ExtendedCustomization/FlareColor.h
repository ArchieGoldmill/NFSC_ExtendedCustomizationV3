#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"
#include "eLightFlare.h"
#include "eView.h"

std::pair<eLightFlareParams, bool> LightFlareParamsBackup = { eLightFlareParams(), false };
void __stdcall SetFlareColor(CarRenderInfo* carRenderInfo, eLightFlare* flare)
{
	Hash flareName = flare->NameHash;
	if (flareName == Hashes::RIGHT_HEADLIGHT || flareName == Hashes::LEFT_HEADLIGHT)
	{
		auto params = eLightFlareParams::List[0];
		if (LightFlareParamsBackup.second)
		{
			*params = LightFlareParamsBackup.first;
		}

		auto color = carRenderInfo->GetHeadlightsColor();
		if (color)
		{
			LightFlareParamsBackup.first = *params;
			LightFlareParamsBackup.second = true;

			Color c(color);
			params->MaxColour[0] = c.Bytes[0];
			params->MaxColour[1] = c.Bytes[1];
			params->MaxColour[2] = c.Bytes[2];
		}
	}
}

void RenderTextureHeadlights(CarRenderInfo* carRenderInfo, eView* a1, D3DXVECTOR4* a2, float a3, D3DXMATRIX* a4, D3DXMATRIX* a5, D3DXMATRIX* a6)
{
	if (carRenderInfo->IsFeEngineOn())
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
	if (carRenderInfo->IsFeEngineOn())
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

void __declspec(naked) FlareColorCave1()
{
	static constexpr auto cExit = 0x007CC601;

	__asm
	{
		push 0x3F800000;

		pushad;
		push esi;
		push[esp + 0x68];
		call SetFlareColor;
		popad;

		jmp cExit;
	}
}

void InitFlareColor()
{
	injector::MakeJMP(0x007CC5FC, FlareColorCave1);
}