#pragma once
#include "CarRenderInfo.h"
#include "Animations.h"
#include "Neon.h"
#include "BrakelightGlow.h"

class CarRenderInfoExtras
{
private:
	CarRenderInfo* carRenderInfo;

public:
	bool IsVisible;
	bMatrix4 CarMatrix;

	CarAnimations Animations;
	CarNeon Neon;
	CarBrakelightGlow BrakelightGlow;

	CarRenderInfoExtras(CarRenderInfo* carRenderInfo) : Animations(carRenderInfo), Neon(carRenderInfo, &CarMatrix), BrakelightGlow(carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
		this->IsVisible = false;
	}
};