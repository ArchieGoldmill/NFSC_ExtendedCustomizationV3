#pragma once
#include "Feature.h"
#include "CarRenderInfo.h"
#include "Game.h"
#include "D3DWrapper.h"
#include "Constants.h"
#include "PartAnimation.h"
#include "PartMarker.h"
#include "DBCarPart.h"

void InitAnimations();

class CarAnimations
{
private:
	CarRenderInfo* carRenderInfo;
	std::vector<PartAnimation*> partAnimations;
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
		auto rideInfo = this->carRenderInfo->RideInfo;

		for (auto slot : AttachSlots)
		{
			auto part = rideInfo->GetPart(slot);
			if (part)
			{
				auto marker = part->GetAttachMarker(rideInfo);
				if (marker)
				{
					this->partMarkers.push_back(new PartMarker(slot, marker));
				}
			}
		}
	}

	void Update()
	{
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
		auto part = this->carRenderInfo->RideInfo->GetPart(slot);
		if (part)
		{
			return part->GetAppliedAttributeIParam(Hashes::MARKER, 0) != 0;
		}

		return false;
	}

private:
	PartAnimation* FindAnimMarkers(Slot slot)
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

	static D3D::Matrix InitLeftDoorMarker(D3DXVECTOR3& a, D3DXVECTOR3& b)
	{
		D3DXVECTOR3 pos;
		pos.x = b.x - 0.05;
		pos.y = b.y;
		pos.z = b.z - (b.z - a.z) / 2;
		auto m = D3D::Matrix::FromRotationZ(-45);
		m.SetW(pos);
		return m;
	}

	static D3D::Matrix InitRightDoorMarker(D3DXVECTOR3& a, D3DXVECTOR3& b)
	{
		D3DXVECTOR3 pos;
		pos.x = b.x - 0.05;
		pos.y = b.y;
		pos.z = b.z - (b.z - a.z) / 2;
		auto m = D3D::Matrix::FromRotationZ(45);
		m.SetW(pos);
		return m;
	}

	static D3D::Matrix InitHoodMarker(D3DXVECTOR3& a, D3DXVECTOR3& b)
	{
		D3DXVECTOR3 pos;
		pos.x = a.x + 0.05;
		pos.y = b.y;
		pos.z = b.z - (b.z - a.z) / 2;
		auto m = D3D::Matrix::FromRotationY(-60);
		m.SetW(pos);
		return m;
	}

	void FindAnimationMarkers()
	{
		this->Clear();

		auto leftHeadlightAnim = this->FindAnimMarkers(Slot::LEFT_HEADLIGHT);
		auto rightHeadlightAnim = this->FindAnimMarkers(Slot::RIGHT_HEADLIGHT);
		auto trunkAnim = this->FindAnimMarkers(Slot_Trunk);
		auto hoodAnim = this->FindAnimMarkers(Slot::HOOD);
		auto leftDoorAnim = this->FindAnimMarkers(Slot::DOOR_LEFT);
		auto rightDoorAnim = this->FindAnimMarkers(Slot::DOOR_RIGHT);

		if (true) // Check config
		{
			if (!leftDoorAnim)
			{
				leftDoorAnim = this->CreateMarker(Slot::DOOR_LEFT, InitLeftDoorMarker);
			}

			if (!rightDoorAnim)
			{
				rightDoorAnim = this->CreateMarker(Slot::DOOR_RIGHT, InitRightDoorMarker);
			}

			if (!hoodAnim)
			{
				hoodAnim = this->CreateMarker(Slot::HOOD, InitHoodMarker);
			}
		}

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
			trunkAnim->AddSubSlot(Slot::SPOILER);
			trunkAnim->AddSubSlot(Slot::REAR_WINDOW);
			trunkAnim->AddSubSlot(Slot::DECAL_REAR_WINDOW);
			//trunkAnim->AddSubSlot(Slot::LICENSE_PLATE);
		}
	}

	PartAnimation* CreateMarker(Slot slot, D3D::Matrix(*func)(D3DXVECTOR3&, D3DXVECTOR3&))
	{
		auto rideInfo = this->carRenderInfo->RideInfo;

		auto part = rideInfo->GetPart(slot);
		if (part)
		{
			auto solid = part->GetSolid();
			if (solid)
			{
				D3DXVECTOR3 a, b;
				solid->GetBoundingBox(&a, &b);

				D3D::Matrix end = func(a, b);
				D3D::Matrix start = D3D::Matrix::FromRotationZ(0);
				start.SetW(end.GetW());

				auto anim = new PartAnimation(slot, start, end);
				this->partAnimations.push_back(anim);
				return anim;
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

		for (auto m : this->partMarkers)
		{
			delete m;
		}

		this->partAnimations.clear();
		this->partMarkers.clear();
	}
};