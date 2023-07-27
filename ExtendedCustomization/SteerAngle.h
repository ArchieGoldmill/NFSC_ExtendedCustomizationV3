#pragma once
#include "Game.h"
#include "Config.h"
#include "Math.h"

class FESteerAngle
{
private:
	float target;

public:
	FESteerAngle()
	{
		*Game::FrontSteerAngle = g_Config.FrontSteerAngle;
	}

	void Update()
	{
		MoveTowards(*Game::FrontSteerAngle, this->target, Game::DeltaTime() * 90);
	}

	void SetTarget(float target)
	{
		this->target = target;
	}

	void Reset()
	{
		this->SetTarget(g_Config.FrontSteerAngle);
	}
};

inline FESteerAngle SteerAngle;