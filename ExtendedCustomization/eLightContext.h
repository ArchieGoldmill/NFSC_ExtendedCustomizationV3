#pragma once
#include <d3dx9.h>
#include "Game.h"

class eLightContext
{
private:
	float data[0x74];

public:
	eLightContext()
	{
		memset(this, 0, 0x1D0);
	}

	void MakePrelit()
	{
		static auto SetupLight = (void(__cdecl*)(eLightContext*, int, D3DXVECTOR3*, int, int, int))0x00746A00;

		D3DXVECTOR3 vec = { 0,0,0 };
		float back = *Game::CarBrightness;
		*Game::CarBrightness = 10;
		SetupLight(this, 0x00A6CC20, &vec, 0, 0x00B1F5F0, 0x00B4AF90);
		*Game::CarBrightness = back;
	}
};