#pragma once
#include "CarRenderInfo.h"
#include "Animations.h"

class CarRenderInfoExtras
{
private:
	CarRenderInfo* carRenderInfo;

public:
	bool IsVisible;
	CarAnimations Animations;

	CarRenderInfoExtras(CarRenderInfo* carRenderInfo) : Animations(carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
		this->IsVisible = false;
	}

	void Update()
	{
		this->Animations.Update();
	}
};