#pragma once
#include "Camber.h"
#include "TrackWidth.h"
#include "Game.h"

void InitStance()
{
	InitCamber();
	InitTrackWidth();

	*Game::FrontSteerAngle = 0;
}