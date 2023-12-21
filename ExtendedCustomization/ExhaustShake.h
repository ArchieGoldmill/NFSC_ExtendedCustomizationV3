#pragma once
#include "PositionMarker.h"

class MarkerShaker
{
private:
	PositionMarker marker;
	PositionMarker* original;
	D3DXVECTOR3 target;
	float time;

public:
	MarkerShaker(PositionMarker* original)
	{
		this->original = original;
		this->marker = *original;

		this->UpdateTarget();
	}

	void Update()
	{
		D3DXVECTOR3 current = { this->marker.Matrix._41,this->marker.Matrix._42 ,this->marker.Matrix._43 };
		if (current != this->target)
		{
			this->time += Game::DeltaTime() * 30;
			if (this->time > 1)
			{
				this->time = 1;
			}

			D3DXVec3Lerp(&current, &current, &this->target, this->time);

			this->marker.Matrix._41 = current.x;
			this->marker.Matrix._42 = current.y;
			this->marker.Matrix._43 = current.z;
		}
		else
		{
			this->UpdateTarget();
		}
	}

	PositionMarker* Get()
	{
		return &this->marker;
	}

private:
	void UpdateTarget()
	{
		float magnitude = 0.4;
		if (Game::InRace())
		{
			float redline = 0.0f;
			DALVehicle_GetRedLine(NULL, &redline, 0);

			float rpm = 0.0f;
			DALVehicle_GetRPM(NULL, &rpm, 0);

			magnitude = rpm / redline;
		}

		if (magnitude < 0.4)
		{
			magnitude = 0.4;
		}


		this->target.x = this->original->Matrix._41;// +this->GetRandom(magnitude / 2);
		this->target.y = this->original->Matrix._42;// +this->GetRandom(magnitude / 2);
		this->target.z = this->original->Matrix._43 + this->GetRandom(magnitude);
		this->time = 0;
	}

	float GetRandom(float magnitude)
	{
		float offset = (bRandom(1000) - 1000) / 60000.0f;

		return offset * magnitude;
	}
};

class CarExhaustShake
{
private:
	CarRenderInfo* carRenderInfo;
	MarkerShaker* LeftShaker = NULL;
	MarkerShaker* RightShaker = NULL;
	MarkerShaker* CenterShaker = NULL;

public:
	CarExhaustShake(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
	}

	void Init()
	{
		if (!this->carRenderInfo->pRideInfo->IsPlayer())
		{
			return;
		}

		if (this->carRenderInfo->Markers.LeftExhaust)
		{
			this->LeftShaker = new MarkerShaker(this->carRenderInfo->Markers.LeftExhaust);
			this->carRenderInfo->Markers.LeftExhaust = this->LeftShaker->Get();
		}

		if (this->carRenderInfo->Markers.RightExhaust)
		{
			this->RightShaker = new MarkerShaker(this->carRenderInfo->Markers.RightExhaust);
			this->carRenderInfo->Markers.RightExhaust = this->RightShaker->Get();
		}

		if (this->carRenderInfo->Markers.CenterExhaust)
		{
			this->CenterShaker = new MarkerShaker(this->carRenderInfo->Markers.CenterExhaust);
			this->carRenderInfo->Markers.CenterExhaust = this->CenterShaker->Get();
		}
	}

	void Update()
	{
		if (IsPaused())
		{
			return;
		}

		if (Game::InFrontEnd() && !this->carRenderInfo->LightsState1)
		{
			return;
		}

		if (this->LeftShaker)
		{
			this->LeftShaker->Update();
		}

		if (this->RightShaker)
		{
			this->RightShaker->Update();
		}

		if (this->CenterShaker)
		{
			this->CenterShaker->Update();
		}
	}

	~CarExhaustShake()
	{
		if (this->LeftShaker)
		{
			delete this->LeftShaker;
			this->LeftShaker = NULL;
		}

		if (this->RightShaker)
		{
			delete this->RightShaker;
			this->RightShaker = NULL;
		}

		if (this->CenterShaker)
		{
			delete this->CenterShaker;
			this->CenterShaker = NULL;
		}
	}
};