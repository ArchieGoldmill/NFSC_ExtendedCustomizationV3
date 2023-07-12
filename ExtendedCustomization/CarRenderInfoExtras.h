#pragma once
#include "CarRenderInfo.h"
#include "Animations.h"
#include "Neon.h"

class CarRenderInfoExtras
{
private:
	CarRenderInfo* carRenderInfo;

public:
	bool IsVisible;
	D3D::Matrix CarMatrix;
	CarAnimations Animations;
	CarNeon Neon;

	CarRenderInfoExtras(CarRenderInfo* carRenderInfo) : Animations(carRenderInfo), Neon(carRenderInfo, &CarMatrix)
	{
		this->carRenderInfo = carRenderInfo;
		this->IsVisible = false;
	}
};