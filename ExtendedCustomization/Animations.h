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
	bool freeMatrices;

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
		auto rightHeadlightAnim = this->FindMarkers(Slot::RIGHT_HEADLIGHT);
		auto trunkAnim = this->FindMarkers(Slot_Trunk);
		auto hoodAnim = this->FindMarkers(Slot::HOOD);
		auto leftDoorAnim = this->FindMarkers(Slot::DOOR_LEFT);
		auto rightDoorAnim = this->FindMarkers(Slot::DOOR_RIGHT);

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

		this->partAnimations.clear();
	}
};