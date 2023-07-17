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
		this->Texture = GetTextureInfo(Hashes::BRAKELIGHT_GLOW, 0, 0);
	}

	void RenderShadow(int a1, float* a2, float a3, int a4, int a5, int a6)
	{
		if (this->Texture && Game::InRace())
		{
			// Backup
			auto backup = this->carRenderInfo->CarShadowTexture;
			auto min = this->carRenderInfo->BoundingBoxMin;
			auto max = this->carRenderInfo->BoundingBoxMax;

			// Draw
			this->carRenderInfo->BoundingBoxMin.x -= 1.3f;
			this->carRenderInfo->BoundingBoxMax.x = this->carRenderInfo->BoundingBoxMin.x + 2.0f;

			this->carRenderInfo->BoundingBoxMax.y *= 1.8f;
			this->carRenderInfo->BoundingBoxMin.y *= 1.8f;

			this->carRenderInfo->CarShadowTexture = this->Texture;
			this->carRenderInfo->DrawAmbientShadow(a1, a2, a3, a4, a5, a6);

			// Restore
			this->carRenderInfo->CarShadowTexture = backup;
			this->carRenderInfo->BoundingBoxMin = min;
			this->carRenderInfo->BoundingBoxMax = max;
		}
	}

	bColor GetColor()
	{
		return bColor(carRenderInfo->IsLeftBrakelightOn() ? 0x80 : 0x49);
	}
};