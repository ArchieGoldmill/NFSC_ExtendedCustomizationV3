#pragma once
#include <vector>
#include "D3DWrapper.h"
#include "Slots.h"
#include "Func.h"
#include "IPartMarker.h"

class PartAnimation : public IPartMarker
{
private:
	D3D::Matrix* start;
	D3D::Matrix* end;
	float amount;
	float target;
	bool freeMatrices;

	std::vector<Slot> subSlots;

public:
	PartAnimation(Slot slot, D3D::Matrix* start, D3D::Matrix* end)
	{
		this->slot = slot;

		this->start = start;
		this->end = end;

		this->amount = 0;
		this->target = 0;

		this->freeMatrices = false;
	}

	PartAnimation(Slot slot, D3D::Matrix& start, D3D::Matrix& end)
	{
		this->slot = slot;

		this->start = new D3D::Matrix(start);
		this->end = new D3D::Matrix(end);

		this->amount = 0;
		this->target = 0;

		this->freeMatrices = true;
	}

	~PartAnimation()
	{
		if (this->freeMatrices)
		{
			delete this->start;
			delete this->end;
		}
	}

	void Start()
	{
		this->amount = 0;
	}

	void End()
	{
		this->amount = 1;
	}

	void SetTarget(float target)
	{
		this->target = target;
	}

	void Toggle()
	{
		if (this->target == 1)
		{
			this->target = 0;
		}
		else
		{
			this->target = 1;
		}
	}

	void Update()
	{
		MoveTowards(this->amount, this->target, Game::DeltaTime() * 2);
	}

	bool IsSlot(Slot slot)
	{
		if (this->slot == slot)
		{
			return true;
		}

		for (auto s : this->subSlots)
		{
			if (s == slot)
			{
				return true;
			}
		}

		return false;
	}

	void AddSubSlot(Slot slot)
	{
		this->subSlots.push_back(slot);
	}

	D3D::Matrix* Get(D3D::Matrix* matrix)
	{
		auto qStart = this->start->ToQuaternion().Normalize();
		auto qEnd = this->end->ToQuaternion().Normalize();

		auto qRotation = D3D::Quaternion::Slerp(qStart, qEnd, this->amount).Normalize();

		auto result = D3D::Matrix::Transformation(qRotation, this->start->GetW());

		auto pos = (this->end->GetW() - this->start->GetW()) * this->amount;
		result.SetW(result.GetW() + pos);

		D3D::Matrix::Multiply(&this->state, &result, matrix);

		return &this->state;
	}
};