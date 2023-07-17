#pragma once
#include "D3DWrapper.h"

struct CameraParams
{
	bMatrix4 Matrix;
	D3DXVECTOR4 Position;
	D3DXVECTOR4 Direction;
	D3DXVECTOR4 Target;
};

struct Camera
{
	CameraParams CurrentKey;
	//CameraParams PreviousKey;
};