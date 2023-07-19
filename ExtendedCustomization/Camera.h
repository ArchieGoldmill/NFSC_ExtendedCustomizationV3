#pragma once
#include "Math.h"

struct CameraParams
{
	D3DXMATRIX Matrix;
	D3DXVECTOR4 Position;
	D3DXVECTOR4 Direction;
	D3DXVECTOR4 Target;
};

struct Camera
{
	CameraParams CurrentKey;
};