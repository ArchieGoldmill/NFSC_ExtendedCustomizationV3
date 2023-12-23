#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"
#include "eLightFlare.h"
#include "eView.h"

std::pair<eLightFlareParams, bool> LightFlareParamsBackup = { eLightFlareParams(), false };
int __stdcall SetFlareColor(CarRenderInfo* carRenderInfo, eLightFlare* flare)
{
	Hash flareName = flare->NameHash;

	if (!carRenderInfo->Extras->Animations->IsLeftHeadlightOpen() && flareName == Hashes::LEFT_HEADLIGHT)
	{
		return 0;
	}

	if (!carRenderInfo->Extras->Animations->IsRightHeadlightOpen() && flareName == Hashes::RIGHT_HEADLIGHT)
	{
		return 0;
	}

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

	return 1;
}

void RenderTextureHeadlights(CarRenderInfo* carRenderInfo, eView* a1, D3DXVECTOR4* a2, float a3, D3DXMATRIX* a4, D3DXMATRIX* a5, D3DXMATRIX* a6)
{
	if (!(carRenderInfo->Extras->Animations->IsLeftHeadlightOpen() || carRenderInfo->Extras->Animations->IsRightHeadlightOpen()))
	{
		return;
	}

	if (g_Config.GetPopUpHeadLights(carRenderInfo->pRideInfo->CarId) == State::Enabled)
	{
		return;
	}

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
	if (g_Config.GetPopUpHeadLights(carRenderInfo->pRideInfo->CarId) == State::Enabled)
	{
		return;
	}

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
	static constexpr auto cExit1 = 0x007CC649;

	__asm
	{
		SAVE_REGS_EAX;
		push esi;
		push[esp + 0x58];
		call SetFlareColor;
		RESTORE_REGS_EAX;

		test eax, eax;
		jne CreateFlare;
		jmp cExit1;

	CreateFlare:
		mov eax, [ebp + 0x18];
		test eax, eax; // restore compare state
		push 0x3F800000;
		jmp cExit;
	}
}

void InitFlareColor()
{
	injector::MakeJMP(0x007CC5FC, FlareColorCave1);
}