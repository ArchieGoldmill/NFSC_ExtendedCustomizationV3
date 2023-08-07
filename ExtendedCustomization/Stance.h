#pragma once
#include "Camber.h"
#include "TrackWidth.h"
#include "Game.h"
#include "TireWidth.h"
#include "RideHeight.h"

void InitStance()
{
	InitCamber();
	InitTrackWidth();
	InitTireWidth();
	InitRideHeight();
}