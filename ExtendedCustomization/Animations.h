#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"
#include "Game.h"
#include "Math.h"
#include "Constants.h"
#include "PartAnimation.h"
#include "PartMarker.h"
#include "DBCarPart.h"
#include "SteeringWheelMarker.h"
#include "ActiveAero.h"

void InitAnimations();

class CarAnimations
{
private:
	CarRenderInfo* carRenderInfo;
	std::vector<IPartAnimation*> partAnimations;
	std::vector<PartMarker*> partMarkers;

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
		this->FindAnimationMarkers();
		auto rideInfo = this->carRenderInfo->pRideInfo;

		for (auto slot : AttachSlots)
		{
			auto part = rideInfo->GetPart(slot);
			if (part && part->HasMarkerName())
			{
				D3DXVECTOR3 scale;
				auto marker = part->GetAttachMarker(rideInfo, &scale);
				if (marker)
				{
					this->partMarkers.push_back(slot == Slot::STEERINGWHEEL ?
						new SteeringWheelMarker(slot, marker, scale, this->carRenderInfo) :
						new PartMarker(slot, marker, scale, this->carRenderInfo));
				}
			}
		}
	}

	void Update()
	{
		if (Game::InRace())
		{
			auto leftHeadlight = (PartAnimation*)this->GetAnimation(Slot::LEFT_HEADLIGHT);
			if (leftHeadlight)
			{
				leftHeadlight->SetTarget(1.0f);
			}

			auto rightHeadlight = (PartAnimation*)this->GetAnimation(Slot::RIGHT_HEADLIGHT);
			if (rightHeadlight)
			{
				rightHeadlight->SetTarget(1.0f);
			}
		}

		for (auto anim : this->partAnimations)
		{
			anim->Update();
		}
	}

	IPartMarker* GetAnimation(Slot slot)
	{
		for (auto anim : this->partAnimations)
		{
			if (anim->IsSlot(slot))
			{
				return anim;
			}
		}

		for (auto m : this->partMarkers)
		{
			if (m->IsSlot(slot))
			{
				return m;
			}
		}

		return NULL;
	}

	bool SlotNeedsMarker(Slot slot)
	{
		for (auto s : AttachSlots)
		{
			if (s == slot)
			{
				auto part = this->carRenderInfo->pRideInfo->GetPart(slot);
				if (part)
				{
					return part->HasMarkerName();
				}
			}
		}

		return false;
	}

	void ResetAllAnimations()
	{
		for (auto anim : this->partAnimations)
		{
			anim->SetTarget(0);
		}

		FEPartAnimation::ResetAll();
	}

private:
	IPartAnimation* FindAnimMarkers(Slot slot)
	{
		auto rideInfo = this->carRenderInfo->pRideInfo;

		auto part = rideInfo->GetPart(slot);
		if (part)
		{
			auto start = part->GetMarker(Hashes::ANIM_START);
			if (start)
			{
				auto end = part->GetMarker(Hashes::ANIM_END);
				if (end)
				{
					IPartAnimation* anim;
					int speed = part->GetAppliedAttributeIParam(Hashes::ACTIVE_AERO, 0);
					if (speed && slot == Slot::SPOILER)
					{
						anim = new ActiveAero(start, end, this->carRenderInfo, (float)speed);
					}
					else
					{
						anim = new PartAnimation(slot, start, end, this->carRenderInfo);
					}

					this->partAnimations.push_back(anim);
					return anim;
				}
			}
		}

		return NULL;
	}

	void FindAnimationMarkers()
	{
		this->Clear();

		auto trunkPart = this->carRenderInfo->pRideInfo->GetPart(Slot_Trunk);
		if (!trunkPart || !trunkPart->GetAppliedAttributeBParam(Hashes::ANIMATE_SPOILER, false))
		{
			this->FindAnimMarkers(Slot::SPOILER);
		}

		auto leftHeadlightAnim = (PartAnimation*)this->FindAnimMarkers(Slot::LEFT_HEADLIGHT);
		auto rightHeadlightAnim = (PartAnimation*)this->FindAnimMarkers(Slot::RIGHT_HEADLIGHT);
		auto trunkAnim = (PartAnimation*)this->FindAnimMarkers(Slot_Trunk);
		auto hoodAnim = (PartAnimation*)this->FindAnimMarkers(Slot::HOOD);
		auto leftDoorAnim = (PartAnimation*)this->FindAnimMarkers(Slot::DOOR_LEFT);
		auto rightDoorAnim = (PartAnimation*)this->FindAnimMarkers(Slot::DOOR_RIGHT);

		if (leftHeadlightAnim)
		{
			leftHeadlightAnim->AddSubSlot(Slot::LEFT_HEADLIGHT_GLASS);
		}

		if (rightHeadlightAnim)
		{
			rightHeadlightAnim->AddSubSlot(Slot::RIGHT_HEADLIGHT_GLASS);
		}

		if (leftDoorAnim)
		{
			leftDoorAnim->AddSubSlot(Slot::FRONT_LEFT_WINDOW);
			leftDoorAnim->AddSubSlot(Slot::LEFT_SIDE_MIRROR);
		}

		if (rightDoorAnim)
		{
			rightDoorAnim->AddSubSlot(Slot::FRONT_RIGHT_WINDOW);
			rightDoorAnim->AddSubSlot(Slot::RIGHT_SIDE_MIRROR);
		}

		if (trunkAnim)
		{
			trunkAnim->AddSubSlot(Slot::REAR_WINDOW);
			trunkAnim->AddSubSlot(Slot::DECAL_REAR_WINDOW);
			trunkAnim->AddSubSlot(Slot::SPOILER);
			trunkAnim->AddSubSlot(Slot::UNIVERSAL_SPOILER_BASE);
			trunkAnim->AddSubSlot(Slot::LICENSE_PLATE);
		}
	}

	void Clear()
	{
		for (auto anim : this->partAnimations)
		{
			delete anim;
		}

		for (auto m : this->partMarkers)
		{
			delete m;
		}

		this->partAnimations.clear();
		this->partMarkers.clear();
	}
};