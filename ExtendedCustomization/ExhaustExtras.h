#pragma once
#include "PositionMarker.h"

class CarExhaust
{
private:
	std::vector<PositionMarker*> markers;

public:

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
};