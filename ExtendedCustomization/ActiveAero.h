#pragma once
#include "IPartMarker.h"
#include "CarRenderInfo.h"
#include "CarRenderConn.h"

class ActiveAero : public IPartAnimation
{
private:
	CarRenderInfo* carRenderInfo;
	float activateSpeed;

public:
	ActiveAero(D3DXMATRIX* start, D3DXMATRIX* end, CarRenderInfo* carRenderInfo, float activateSpeed)
	{
		this->start = start;
		this->end = end;
		this->carRenderInfo = carRenderInfo;
		this->activateSpeed = activateSpeed;
		this->slot = Slot::SPOILER;
	}

	void Update()
	{
		if (Game::InRace())
		{
			auto carRenderConn = CarRenderConn::Get(this->carRenderInfo);
			if (carRenderConn)
			{
				auto pVehicle = carRenderConn->GetPVehicle();
				if (pVehicle)
				{
					float speed = pVehicle->GetSpeed();
					float moveSpeed = Game::DeltaTime();
					if (speed > this->activateSpeed)
					{
						MoveTowards(this->amount, 1.0f, moveSpeed);
					}
					else
					{
						MoveTowards(this->amount, 0.0f, moveSpeed);
					}
				}
			}
		}
	}

	D3DXMATRIX* Get(D3DXMATRIX* matrix)
	{
		if (!this->amount)
		{
			return matrix;
		}

		this->state = *matrix;
		auto pos = (GetVector3(this->end, 3) - GetVector3(this->start, 3)) * this->amount;
		SetVector3(&this->state, 3, GetVector3(&this->state, 3) + pos);

		return &this->state;
	}

	bool IsSlot(Slot slot)
	{
		return this->slot == slot;
	}

	void SetTarget(float)
	{
		// do nothing
	}

	void AddSubSlot(Slot)
	{
		// do nothing
	}

	D3DXMATRIX Calculate()
	{
		MessageBoxA(NULL, "Not expected to call calculate for ActiveAero!", "Extended Customization", MB_ICONERROR);
		exit(1);
	}
};