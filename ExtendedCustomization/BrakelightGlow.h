#pragma once
#include "TextureInfo.h"
#include "Hashes.h"
#include "CarRenderInfo.h"

class CarBrakelightGlow
{
private:
	CarRenderInfo* carRenderInfo;

public:
	TextureInfo* Texture = NULL;

	CarBrakelightGlow(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
		this->Texture = TextureInfo::Get(Hashes::BRAKELIGHT_GLOW, 0, 0);
	}

	void RenderShadow(eView* a1, D3DXVECTOR4* a2, float a3, D3DXMATRIX* a4, D3DXMATRIX* a5, D3DXMATRIX* a6)
	{
		bool isDamaged = this->carRenderInfo->Damage.IsBrakelightLeftDamaged() || this->carRenderInfo->Damage.IsBrakelightRightDamaged();
		if (this->Texture && !isDamaged)
		{
			// Backup
			auto backup = this->carRenderInfo->CarShadowTexture;
			auto min = this->carRenderInfo->BoundingBoxMin;
			auto max = this->carRenderInfo->BoundingBoxMax;
			auto shadowShift = g_Config.ShadowShiftMult;

			// Draw
			this->carRenderInfo->BoundingBoxMin.x -= 1.3f;
			this->carRenderInfo->BoundingBoxMax.x = this->carRenderInfo->BoundingBoxMin.x + 2.0f;

			this->carRenderInfo->BoundingBoxMax.y *= 1.8f;
			this->carRenderInfo->BoundingBoxMin.y *= 1.8f;

			this->carRenderInfo->CarShadowTexture = this->Texture;
			g_Config.ShadowShiftMult = 0;
			this->carRenderInfo->DrawAmbientShadow(a1, a2, a3, a4, a5, a6);

			// Restore
			this->carRenderInfo->CarShadowTexture = backup;
			this->carRenderInfo->BoundingBoxMin = min;
			this->carRenderInfo->BoundingBoxMax = max;
			g_Config.ShadowShiftMult = shadowShift;
		}
	}

	Color GetColor()
	{
		return Color(carRenderInfo->IsGlareOn(VehicleFX_BRAKELIGHTS) ? 0x80808080 : (this->carRenderInfo->IsLightOn() ? 0x69696969 : 0));
	}
};