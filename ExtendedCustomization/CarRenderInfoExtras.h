#pragma once
#include "CarRenderInfo.h"
#include "Animations.h"
#include "Neon.h"
#include "BrakelightGlow.h"
#include "Paints.h"
#include "LicensePlateText.h"
#include "RotorGlow.h"
#include "ExhaustFX.h"
#include "ExhaustShake.h"
#include "WheelTextures.h"
#include "CarLightTrails.h"

class CarRenderInfoExtras
{
private:
	CarRenderInfo* carRenderInfo;
	bool initialStart;

public:
	bool IsVisible;
	bool IsEngineOn;
	D3DXMATRIX CarMatrix;
	D3DXMATRIX CarMatrixReflection;

	CarAnimations* Animations = NULL;
	CarNeon* Neon = NULL;
	CarBrakelightGlow* BrakelightGlow = NULL;
	CarPaint* Paint = NULL;
	CarExhaustFX* ExhaustFX = NULL;
	CarExhaustShake* ExhaustShake = NULL;
	CarLicensePlateText* LicensePlateText = NULL;
	CarRotorGlow* RotorGlow = NULL;
	CarWheelTextures* WheelTextures = NULL;
	CarLightTrails* LightTrails = NULL;

	CarRenderInfoExtras(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
		this->IsVisible = false;
		this->IsEngineOn = false;
		this->initialStart = true;

		if (g_Config.LicensePlateText)
		{
			this->LicensePlateText = new CarLicensePlateText(carRenderInfo);
		}

		if (g_Config.RotorGlow)
		{
			this->RotorGlow = new CarRotorGlow(carRenderInfo);
		}

		if (g_Config.Neon)
		{
			this->Neon = new CarNeon(carRenderInfo, &this->CarMatrix, &this->CarMatrixReflection);
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

		if (g_Config.LightTrailSpeed)
		{
			this->LightTrails = new CarLightTrails(carRenderInfo);
		}
	}

	void CheckEngine()
	{
		if (Game::InRace() && !this->IsEngineOn && this->initialStart)
		{
			if (this->Animations)
			{
				auto leftHeadlight = (PartAnimation*)this->Animations->GetAnimation(Slot::LEFT_HEADLIGHT);
				if (leftHeadlight)
				{
					leftHeadlight->SetTarget(1.0f);
				}

				auto rightHeadlight = (PartAnimation*)this->Animations->GetAnimation(Slot::RIGHT_HEADLIGHT);
				if (rightHeadlight)
				{
					rightHeadlight->SetTarget(1.0f);
				}
			}

			this->IsEngineOn = true;
			this->initialStart = false;
		}
	}

	~CarRenderInfoExtras()
	{
		if (this->LicensePlateText)
		{
			delete this->LicensePlateText;
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

		if (this->WheelTextures)
		{
			delete this->WheelTextures;
		}
		
		if (this->LightTrails)
		{
			delete this->LightTrails;
		}
	}
};