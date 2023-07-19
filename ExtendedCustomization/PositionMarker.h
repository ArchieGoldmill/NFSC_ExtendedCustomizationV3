#pragma once
#include "Hashes.h"
#include "Math.h"

struct PositionMarker
{
	Hash Hash;
	int blank[3];
	D3DXMATRIX Matrix;
};