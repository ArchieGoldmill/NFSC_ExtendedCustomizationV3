#pragma once
#include "CarRenderInfo.h"
#include "Animations.h"
#include "Neon.h"
#include "BrakelightGlow.h"
#include "PaintExtras.h"

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

	CarRenderInfoExtras(CarRenderInfo* carRenderInfo) : Animations(carRenderInfo), Neon(carRenderInfo, &CarMatrix), BrakelightGlow(carRenderInfo), Paint(carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
		this->IsVisible = false;
	}
};