#pragma once
#include "PartMarker.h"
#include "CarRenderConn.h"

class SteeringWheelMarker : public PartMarker
{
private:
	float angle;

public:
	SteeringWheelMarker(Slot slot, D3DXMATRIX* marker, D3DXVECTOR3 scale, CarRenderInfo* carRenderInfo) : PartMarker(slot, marker, scale, carRenderInfo)
	{
		this->angle = 0;
	}

	D3DXMATRIX* Get(D3DXMATRIX* matrix)
	{
		D3DXMATRIX scale;
		auto part = this->carRenderInfo->pRideInfo->GetPart(this->slot);
		if (part && part->IsStock())
		{
			D3DXMatrixIdentity(&scale);
		}
		else
		{
			D3DXMatrixScaling(&scale, this->scale.x, this->scale.y, this->scale.z);
		}

		D3DXMATRIX rotationX;
		this->GetAngle();
		D3DXMatrixRotationX(&rotationX, this->angle);

		D3DXMatrixIdentity(&this->state);
		D3DXMatrixMultiply(&this->state, &rotationX, &this->state);
		D3DXMatrixMultiply(&this->state, &scale, &this->state);
		D3DXMatrixMultiply(&this->state, &this->state, this->marker);
		D3DXMatrixMultiply(&this->state, &this->state, matrix);

		return &this->state;
	}

private:
	void GetAngle()
	{
		if (Game::InRace())
		{
			auto conn = CarRenderConn::Get(this->carRenderInfo);
			if (conn)
			{
				float a = conn->LeftWheelRotation > 0 ? conn->LeftWheelRotation : conn->RightWheelRotation;
				a = conn->RightWheelRotation < 0 ? conn->RightWheelRotation : conn->LeftWheelRotation;
				a = D3DXToRadian(a * -120.0f);
				MoveTowards(this->angle, a, Game::DeltaTime() * 2);
			}
		}
	}
};