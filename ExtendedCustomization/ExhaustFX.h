#pragma once
#include <vector>
#include "CarRenderInfo.h"
#include "Effect.h"
#include "Config.h"

void InitExhaust();

class CarExhaustFX
{
private:
	CarRenderInfo* carRenderInfo;
	std::vector<PositionMarker*> markers;

public:
	std::vector<Effect*> Effects;

	CarExhaustFX(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
	}

	PositionMarker* GetAdjustedMarker(PositionMarker* fxMarker, PositionMarker* exhaustMarker, bool flip)
	{
		auto marker = new PositionMarker();
		marker->Hash = fxMarker->Hash;

		if (flip)
		{
			D3DXMATRIX flipped = fxMarker->Matrix;
			D3DXMATRIX scale;
			D3DXMatrixScaling(&scale, 1, -1, 1);

			D3DXMatrixMultiply(&flipped, &flipped, &scale);
			D3DXMatrixMultiply(&marker->Matrix, &flipped, &exhaustMarker->Matrix);
		}
		else
		{
			D3DXMatrixMultiply(&marker->Matrix, &fxMarker->Matrix, &exhaustMarker->Matrix);
		}

		this->markers.push_back(marker);

		return marker;
	}

	void CreateEffect(D3DXMATRIX* matrix)
	{
		if (g_Config.ExhaustSmoke)
		{
			this->Effects.push_back(new Effect(matrix));
		}
	}

	~CarExhaustFX()
	{
		for (auto& marker : this->markers)
		{
			delete marker;
		}

		for (auto& effect : this->Effects)
		{
			delete effect;
		}

		this->markers.clear();
		this->Effects.clear();
	}
};