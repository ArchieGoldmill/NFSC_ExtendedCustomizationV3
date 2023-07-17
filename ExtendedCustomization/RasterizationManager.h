#pragma once
#include "RideInfo.h"
#include "Func.h"

struct RasterizationManager
{
	char unk[0x5C];
	bool Mirror;

	char Initialize(TextureInfo* texture, int canvasWidth, int canvasHeight, RideInfo* rideInfo, char vinylNum, char a7, int a8)
	{
		FUNC(0x007D09D0, char, __thiscall, _Initialize, RasterizationManager*, TextureInfo*, signed int, signed int, RideInfo*, char, char, int);
		return _Initialize(this, texture, canvasWidth, canvasHeight, rideInfo, vinylNum, a7, a8);
	}
};