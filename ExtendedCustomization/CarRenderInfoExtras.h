#pragma once
#include "CarRenderInfo.h"
#include "Animations.h"
#include "Neon.h"
#include "BrakelightGlow.h"
#include "PaintExtras.h"
#include "ExhaustExtras.h"
#include "TextureExtras.h"

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

	CarRenderInfoExtras(CarRenderInfo* carRenderInfo) : Animations(carRenderInfo), Neon(carRenderInfo, &CarMatrix), BrakelightGlow(carRenderInfo), Paint(carRenderInfo),
		Exhaust(carRenderInfo), Textures(carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
		this->IsVisible = false;
	}
};