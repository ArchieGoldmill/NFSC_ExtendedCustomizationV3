#pragma once
#include "Camber.h"
#include "TrackWidth.h"
#include "Game.h"
#include "TireWidth.h"

void InitStance()
{
	InitCamber();
	InitTrackWidth();
	InitTireWidth();

	*Game::FrontSteerAngle = g_Config.FrontSteerAngle;
}