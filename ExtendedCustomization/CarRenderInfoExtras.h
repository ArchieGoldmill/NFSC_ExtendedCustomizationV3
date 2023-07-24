#pragma once
#include "CarRenderInfo.h"
#include "Animations.h"
#include "Neon.h"
#include "BrakelightGlow.h"
#include "Paints.h"
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

	CarAnimations* Animations = NULL;
	CarNeon* Neon = NULL;
	CarBrakelightGlow* BrakelightGlow = NULL;
	CarPaint* Paint = NULL;
	CarExhaustFX* ExhaustFX = NULL;
	CarExhaustShake* ExhaustShake = NULL;
	CarTextures* Textures = NULL;
	CarRotorGlow* RotorGlow = NULL;

	CarRenderInfoExtras(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
		this->IsVisible = false;

		this->Textures = new CarTextures(carRenderInfo);

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

		if (g_Config.CustomPaints)
		{
			this->Paint = new CarPaint(carRenderInfo);
		}

		if (g_Config.PartAnimations)
		{
			this->Animations = new CarAnimations(carRenderInfo);
		}
	}

	~CarRenderInfoExtras()
	{
		if (this->Textures)
		{
			delete this->Textures;
		}

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
		
		if (this->Paint)
		{
			delete this->Paint;
		}
		
		if (this->Animations)
		{
			delete this->Animations;
		}
	}
};