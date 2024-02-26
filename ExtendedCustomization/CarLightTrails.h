#pragma once
#include <vector>
#include "Effect.h"
#include "CarRenderInfo.h"

class CarLightTrails
{
private:
	std::vector<Effect*> effects;
	CarRenderInfo* carRenderInfo;

public:
	CarLightTrails(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
	}

	void CreateEffect(D3DXMATRIX* matrix)
	{
		this->effects.push_back(new Effect(matrix));
	}

	void Update(D3DXMATRIX* matrix, float a3, D3DXVECTOR3* velocity)
	{
		for (auto effect : this->effects)
		{
			effect->Update(matrix, VltHashes::fxcar_lighttrail, a3, 1.0f, velocity);
			effect = effect->Next;
		}
	}

	~CarLightTrails()
	{
		for (auto& effect : this->effects)
		{
			delete effect;
		}

		this->effects.clear();
	}
};