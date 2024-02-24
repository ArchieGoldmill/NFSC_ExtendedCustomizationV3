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
	std::vector<Effect*> SmokeEffects;

public:

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

	void CreateSmokeEffect(D3DXMATRIX* matrix)
	{
		if (g_Config.ExhaustSmoke)
		{
			this->SmokeEffects.push_back(new Effect(matrix));
		}
	}

	void UpdateSmoke(D3DXMATRIX* matrix, float a3, D3DXVECTOR3* velocity)
	{
		if (this->carRenderInfo->IsEngineOn())
		{
			for (auto effect : this->SmokeEffects)
			{
				effect->Update(matrix, 0x3D42B5F3, a3, 1.0f, velocity);
				effect = effect->Next;
			}
		}
	}

	~CarExhaustFX()
	{
		for (auto& marker : this->markers)
		{
			delete marker;
		}

		for (auto& effect : this->SmokeEffects)
		{
			delete effect;
		}

		this->markers.clear();
		this->SmokeEffects.clear();
	}
};