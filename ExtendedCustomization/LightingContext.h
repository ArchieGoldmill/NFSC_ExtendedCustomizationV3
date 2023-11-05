#pragma once
#include <d3dx9.h>
#include "Game.h"

struct LightingContext
{
	int type;
};

struct DynamicLightingContext : public LightingContext
{
	D3DXVECTOR3 padv3;
	D3DXMATRIX local_color;
	D3DXMATRIX local_direction;
	D3DXMATRIX local_eye;
	D3DXVECTOR4 envmap_pos;
	D3DXVECTOR4 idk_vector4;
	D3DXMATRIX envmap_rotation;
	D3DXVECTOR4 harmonics[10];
};