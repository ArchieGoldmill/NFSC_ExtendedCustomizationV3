#pragma once
#include <vector>
#include "Math.h"
#include "Slots.h"
#include "Func.h"
#include "IPartMarker.h"

struct FEPartAnimation
{
	Slot SlotId;
	float Amount;
	float Target;
	bool Enabled;

	static inline std::vector<FEPartAnimation> List;

	static inline Slot LastSlot = Slot::INVALID;

	static FEPartAnimation* Get(Slot slot)
	{
		for (auto& part : List)
		{
			if (part.SlotId == slot)
			{
				return &part;
			}
		}

		return NULL;
	}

	static void Create(Slot slot)
	{
		auto part = Get(slot);
		if (!part)
		{
			List.push_back({ slot, 0, 0, false });
			part = &List[List.size() - 1];
		}

		part->Enabled = true;
		part->Target = 1.0;
	}

	static void ResetAll()
	{
		List.clear();
	}
};

class PartAnimation : public IPartMarker
{
private:
	D3DXMATRIX* start;
	D3DXMATRIX* end;
	float amount;
	float target;

	std::vector<Slot> subSlots;

public:
	PartAnimation(Slot slot, D3DXMATRIX* start, D3DXMATRIX* end)
	{
		this->slot = slot;

		this->amount = 0;
		this->target = 0;

		if (Game::InFrontEnd())
		{
			auto backup = FEPartAnimation::Get(slot);
			if (backup && backup->Enabled)
			{
				this->target = backup->Target;
				this->amount = backup->Amount;
			}
		}

		this->start = start;
		this->end = end;
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
		if (Game::InFrontEnd())
		{
			auto backup = FEPartAnimation::Get(this->slot);
			if (backup && backup->Enabled)
			{
				backup->Amount = this->amount;
				this->target = backup->Target;
			}
		}
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

	D3DXMATRIX* Get(D3DXMATRIX* matrix)
	{
		D3DXQUATERNION qStart;
		D3DXQuaternionRotationMatrix(&qStart, this->start);
		D3DXQuaternionNormalize(&qStart, &qStart);

		D3DXQUATERNION qEnd;
		D3DXQuaternionRotationMatrix(&qEnd, this->end);
		D3DXQuaternionNormalize(&qEnd, &qEnd);

		D3DXQUATERNION qRotation;
		D3DXQuaternionSlerp(&qRotation, &qStart, &qEnd, this->amount);
		D3DXQuaternionNormalize(&qRotation, &qRotation);

		D3DXVECTOR3 origin = GetVector3(this->start, 3);
		D3DXMATRIX result;
		D3DXMatrixTransformation(&result, NULL, NULL, NULL, &origin, &qRotation, NULL);

		auto pos = (GetVector3(this->end, 3) - GetVector3(this->start, 3)) * this->amount;
		SetVector3(&result, 3, GetVector3(&result, 3) + pos);

		D3DXMatrixMultiply(&this->state, &result, matrix);

		return &this->state;
	}
};