#pragma once
#include "CarRenderInfo.h"

void InitExhaust();

class CarExhaustFX
{
private:
	CarRenderInfo* carRenderInfo;
	std::vector<PositionMarker*> markers;

public:
	CarExhaustFX(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
	}

	PositionMarker* GetAdjustedMarker(PositionMarker* fxMarker, PositionMarker* exhaustMarker, bool flip)
	{
		auto marker = new PositionMarker();

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

	~CarExhaustFX()
	{
		for (auto& marker : this->markers)
		{
			delete marker;
		}

		this->markers.clear();
	}
};