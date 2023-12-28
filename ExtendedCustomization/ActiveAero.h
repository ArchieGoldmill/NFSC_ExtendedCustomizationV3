#pragma once
#include "IPartMarker.h"
#include "CarRenderInfo.h"
#include "CarRenderConn.h"

class ActiveAero : public PartAnimation
{
private:
	float activateSpeed;

public:
	ActiveAero(D3DXMATRIX* start, D3DXMATRIX* end, CarRenderInfo* carRenderInfo, float activateSpeed) : PartAnimation(slot, start, end, carRenderInfo)
	{
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
					float speed = pVehicle->GetSpeed() * 3.5999999f;
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
};