#pragma once
#include "CarRenderInfo.h"
#include "Animations.h"
#include "Neon.h"
#include "BrakelightGlow.h"
#include "PaintExtras.h"
#include "ExhaustExtras.h"
#include "TextureExtras.h"
#include "RotorGlow.h"

class CarRenderInfoExtras
{
private:
	CarRenderInfo* carRenderInfo;

public:
	bool IsVisible;
	D3DXMATRIX CarMatrix;

	CarAnimations Animations;
	CarNeon Neon;
	CarBrakelightGlow BrakelightGlow;
	CarPaint Paint;
	CarExhaust Exhaust;
	CarTextures Textures;
	CarRotorGlow* RotorGlow = NULL;

	CarRenderInfoExtras(CarRenderInfo* carRenderInfo) : Animations(carRenderInfo), Neon(carRenderInfo, &CarMatrix), BrakelightGlow(carRenderInfo), Paint(carRenderInfo),
		Exhaust(carRenderInfo), Textures(carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
		this->IsVisible = false;

		if (g_Config.RotorGlow)
		{
			this->RotorGlow = new CarRotorGlow(carRenderInfo);
		}
	}

	~CarRenderInfoExtras()
	{
		if (this->RotorGlow)
		{
			delete this->RotorGlow;
		}
	}
};