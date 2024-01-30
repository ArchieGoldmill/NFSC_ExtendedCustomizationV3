#pragma once
#include "CarRenderInfo.h"
#include "Config.h"

void InitRotorGlow();

struct RotorMarkerStorage
{
	D3DXMATRIX Markers[4];
	int Count = 0;

	void SetMarker(D3DXMATRIX* marker)
	{
		if (this->Count > 3)
		{
			return;
		}

		this->Markers[this->Count++] = *marker;
	}

	void RenderMarkers(float rotorRadius, float rotorOffset, float temp, TextureInfo* texture)
	{
		if (!rotorRadius || !this->Count)
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
			for (auto& marker : this->Markers)
			{
				ePoly poly;
				poly.Vertices[0].x = rotorRadius;
				poly.Vertices[0].y = rotorOffset;
				poly.Vertices[0].z = rotorRadius;

				poly.Vertices[1].x = rotorRadius;
				poly.Vertices[1].y = rotorOffset;
				poly.Vertices[1].z = -rotorRadius;

				poly.Vertices[2].x = -rotorRadius;
				poly.Vertices[2].y = rotorOffset;
				poly.Vertices[2].z = -rotorRadius;

				poly.Vertices[3].x = -rotorRadius;
				poly.Vertices[3].y = rotorOffset;
				poly.Vertices[3].z = rotorRadius;

				Color color;
				color.Bytes[0] = 0xFF;
				color.Bytes[1] = 0xFF;
				color.Bytes[2] = 0xFF;
				color.Bytes[3] = 0xFF * alpha;
				poly.SetColor(color);
				poly.Render(texture, &marker);
			}
		}

		this->Clear();
	}

	void Clear()
	{
		this->Count = 0;
	}
};

class CarRotorGlow
{
private:
	CarRenderInfo* carRenderInfo;
	float rotorRadius;
	float rotorOffset;
	float temp;
	TextureInfo* texture;
	RotorMarkerStorage MarkerStorage;
	RotorMarkerStorage MarkerReflectionStorage;

public:
	CarRotorGlow(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
		this->texture = TextureInfo::Get(Hashes::ROTORS_GLOW, 0, 0);
	}

	void Init()
	{
		auto frontRotor = this->carRenderInfo->pRideInfo->GetPart(Slot::FRONT_ROTOR);
		if (frontRotor)
		{
			float rotorWidth = frontRotor->GetAppliedAttributeFParam(Hashes::WIDTH, 0.0f);
			auto rotorModel = this->carRenderInfo->PartModel[(int)Slot::FRONT_ROTOR][0];
			if (rotorModel && rotorWidth)
			{
				D3DXVECTOR3 a, b;
				rotorModel->GetBoundingBox(&a, &b);

				this->rotorRadius = (b.z - a.z) / 2;
				this->rotorOffset = b.y - rotorWidth;
			}
		}

		this->temp = 0;
	}

	void SetMarker(D3DXMATRIX* marker, bool reflection)
	{
		if (reflection)
		{
			this->MarkerReflectionStorage.SetMarker(marker);
		}
		else
		{
			this->MarkerStorage.SetMarker(marker);
		}
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

	void RenderMarkers(bool reflection)
	{
		RotorMarkerStorage* storage = reflection ? &this->MarkerReflectionStorage : &this->MarkerStorage;
		storage->RenderMarkers(this->rotorRadius, this->rotorOffset, this->temp, this->texture);
	}
};