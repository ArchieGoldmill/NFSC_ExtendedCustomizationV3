#pragma once
#include "CarRenderInfo.h"
#include "Config.h"

void InitRotorGlow();

class CarRotorGlow
{
private:
	CarRenderInfo* carRenderInfo;
	D3DXMATRIX markers[4];
	int markerCount;
	float rotorRadius;
	float rotorOffset;
	float temp;
	TextureInfo* texture;

public:
	CarRotorGlow(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
		this->texture = GetTextureInfo(Hashes::ROTORS_GLOW, 0, 0);
	}

	void Init()
	{
		float rotorWidth = 0.04f;
		auto frontRotor = this->carRenderInfo->RideInfo->GetPart(Slot::FRONT_ROTOR);
		if (frontRotor)
		{
			rotorWidth = frontRotor->GetAppliedAttributeFParam(Hashes::WIDTH, rotorWidth);
			auto rotorModel = this->carRenderInfo->PartModel[(int)Slot::FRONT_ROTOR][0];
			if (rotorModel)
			{
				D3DXVECTOR3 a, b;
				rotorModel->GetBoundingBox(&a, &b);

				this->rotorRadius = (b.z - a.z) / 2;
				this->rotorOffset = b.y - rotorWidth;
			}
		}

		this->temp = 0;
	}

	void SetMarker(D3DXMATRIX* marker)
	{
		if (this->markerCount > 3)
		{
			return;
		}

		this->markers[this->markerCount++] = *marker;
	}

	void Update()
	{
		if (Game::InRace() && !IsPaused())
		{
			auto conn = CarRenderConn::Get(this->carRenderInfo);
			if (conn)
			{
				auto pvehicle = conn->GetPVehicle();
				if (pvehicle)
				{
					float speed = pvehicle->GetSpeed();
					float braking = pvehicle->GetBraking();
					float accel = pvehicle->GetAccelerating();
					if (braking > 0 && speed > 0.0f && this->temp < 10.0f)
					{
						float step = Game::DeltaTime() * braking;
						bool isBurnOut = accel > 0 && braking > 0;
						if (!isBurnOut)
						{
							step *= pow(speed / 25, 2) * 2;
						}

						this->temp += step;
					}
					else if (this->temp > -1.0f)
					{
						this->temp -= Game::DeltaTime() * 0.3f;
					}
				}
			}
		}
	}

	void Render()
	{
		if (!this->rotorRadius)
		{
			return;
		}

		float alpha = temp > 1.0f ? 1.0f : temp;
		if (alpha < 0)
		{
			alpha = 0;
		}

		if (g_Config.DebugRotorGlow)
		{
			alpha = 1.0f;
		}

		if (alpha > 0)
		{
			for (auto& marker : this->markers)
			{
				ePoly poly;
				poly.Vertices[0].x = this->rotorRadius;
				poly.Vertices[0].y = this->rotorOffset;
				poly.Vertices[0].z = this->rotorRadius;

				poly.Vertices[1].x = this->rotorRadius;
				poly.Vertices[1].y = this->rotorOffset;
				poly.Vertices[1].z = -this->rotorRadius;

				poly.Vertices[2].x = -this->rotorRadius;
				poly.Vertices[2].y = this->rotorOffset;
				poly.Vertices[2].z = -this->rotorRadius;

				poly.Vertices[3].x = -this->rotorRadius;
				poly.Vertices[3].y = this->rotorOffset;
				poly.Vertices[3].z = this->rotorRadius;

				Color color;
				color.Bytes[0] = 0xFF;
				color.Bytes[1] = 0xFF;
				color.Bytes[2] = 0xFF;
				color.Bytes[3] = 0xFF * alpha;
				poly.SetColor(color);
				poly.Render(this->texture, &marker);
			}
		}

		this->Clear();
	}

	void Clear()
	{
		this->markerCount = 0;
	}
};