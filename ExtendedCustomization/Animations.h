#pragma once
#include <vector>
#include "Feature.h"
#include "CarRenderInfo.h"
#include "Game.h"
#include "D3DWrapper.h"
#include "Constants.h"

void InitAnimations();

class PartAnimation
{
private:
	D3D::Matrix* start;
	D3D::Matrix* end;
	D3D::Matrix state;
	float amount;
	float target;

	Slot slot;
	std::vector<Slot> subSlots;

public:
	PartAnimation(Slot slot, D3D::Matrix* start, D3D::Matrix* end)
	{
		this->slot = slot;

		this->start = start;
		this->end = end;

		this->amount = 0;
		this->target = 0;
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
		MoveTowards(this->amount, this->target, *Game::DeltaTime * 2);
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

class CarAnimations
{
private:
	CarRenderInfo* carRenderInfo;
	std::vector<PartAnimation*> partAnimations;

public:
	CarAnimations(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
	}

	~CarAnimations()
	{
		this->Clear();
	}

	void FindMarkers()
	{
		this->Clear();

		auto leftHeadlightAnim = this->FindMarkers(Slot::LEFT_HEADLIGHT);
		if (leftHeadlightAnim)
		{
			leftHeadlightAnim->AddSubSlot(Slot::LEFT_HEADLIGHT_GLASS);
		}

		auto rightHeadlightAnim = this->FindMarkers(Slot::RIGHT_HEADLIGHT);
		if (rightHeadlightAnim)
		{
			rightHeadlightAnim->AddSubSlot(Slot::RIGHT_HEADLIGHT_GLASS);
		}

		auto trunkAnim = this->FindMarkers(Slot_Trunk);
		if (trunkAnim)
		{
			trunkAnim->AddSubSlot(Slot::SPOILER);
			trunkAnim->AddSubSlot(Slot::REAR_WINDOW);
			trunkAnim->AddSubSlot(Slot::DECAL_REAR_WINDOW);
			//trunkAnim->AddSubSlot(Slot::LICENSE_PLATE);
		}

		this->FindMarkers(Slot::HOOD);

		auto leftDoorAnim = this->FindMarkers(Slot::DOOR_LEFT);
		if (leftDoorAnim)
		{
			leftDoorAnim->AddSubSlot(Slot::FRONT_LEFT_WINDOW);
			leftDoorAnim->AddSubSlot(Slot::LEFT_SIDE_MIRROR);
		}

		auto rightDoorAnim = this->FindMarkers(Slot::DOOR_RIGHT);
		if (rightDoorAnim)
		{
			rightDoorAnim->AddSubSlot(Slot::FRONT_RIGHT_WINDOW);
			rightDoorAnim->AddSubSlot(Slot::RIGHT_SIDE_MIRROR);
		}
	}

	void Update()
	{
		for (auto anim : this->partAnimations)
		{
			anim->Update();
		}
	}

	PartAnimation* GetAnimation(Slot slot)
	{
		for (auto anim : this->partAnimations)
		{
			if (anim->IsSlot(slot))
			{
				return anim;
			}
		}

		return NULL;
	}

private:
	PartAnimation* FindMarkers(Slot slot)
	{
		auto rideInfo = this->carRenderInfo->RideInfo;

		auto part = rideInfo->GetPart(slot);
		if (part)
		{
			auto start = part->GetMarker(Hashes::ANIM_START);
			if (start)
			{
				auto end = part->GetMarker(Hashes::ANIM_END);
				if (end)
				{
					auto anim = new PartAnimation(slot, start, end);
					this->partAnimations.push_back(anim);
					return anim;
				}
			}
		}

		return NULL;
	}

	void Clear()
	{
		for (auto anim : this->partAnimations)
		{
			delete anim;
		}

		this->partAnimations.clear();
	}
};