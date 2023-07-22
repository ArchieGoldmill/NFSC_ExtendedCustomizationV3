#pragma once
#include "CarRenderInfo.h"
#include "Animations.h"
#include "Neon.h"
#include "BrakelightGlow.h"
#include "PaintExtras.h"
#include "TextureExtras.h"
#include "RotorGlow.h"
#include "ExhaustFX.h"
#include "ExhaustShake.h"

class CarRenderInfoExtras
{
private:
	CarRenderInfo* carRenderInfo;

public:
	bool IsVisible;
	D3DXMATRIX CarMatrix;

	CarAnimations Animations;
	CarNeon* Neon = NULL;
	CarBrakelightGlow* BrakelightGlow = NULL;
	CarPaint Paint;
	CarExhaustFX* ExhaustFX = NULL;
	CarExhaustShake* ExhaustShake = NULL;
	CarTextures Textures;
	CarRotorGlow* RotorGlow = NULL;

	CarRenderInfoExtras(CarRenderInfo* carRenderInfo) : Animations(carRenderInfo), Paint(carRenderInfo), Textures(carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
		this->IsVisible = false;

		if (g_Config.RotorGlow)
		{
			this->RotorGlow = new CarRotorGlow(carRenderInfo);
		}

		if (g_Config.Neon)
		{
			this->Neon = new CarNeon(carRenderInfo, &this->CarMatrix);
		}

		if (g_Config.BrakelightGlow)
		{
			this->BrakelightGlow = new CarBrakelightGlow(carRenderInfo);
		}

		if (g_Config.ExhaustShake)
		{
			this->ExhaustShake = new CarExhaustShake(carRenderInfo);
		}

		if (g_Config.FixAutosculptExhaustFX)
		{
			this->ExhaustFX = new CarExhaustFX(carRenderInfo);
		}
	}

	~CarRenderInfoExtras()
	{
		if (this->RotorGlow)
		{
			delete this->RotorGlow;
		}

		if (this->Neon)
		{
			delete this->Neon;
		}

		if (this->BrakelightGlow)
		{
			delete this->BrakelightGlow;
		}

		if (this->ExhaustShake)
		{
			delete this->ExhaustShake;
		}

		if (this->ExhaustFX)
		{
			delete this->ExhaustFX;
		}
	}
};