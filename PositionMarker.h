#pragma once
#include "Hashes.h"
#include "D3DWrapper.h"

struct PositionMarker
{
	Hash Hash;
	int blank[3];
	D3D::Matrix Matrix;
};