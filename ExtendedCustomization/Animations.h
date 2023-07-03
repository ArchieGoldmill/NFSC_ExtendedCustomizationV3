#pragma once
#include <vector>
#include "Feature.h"
#include "CarRenderInfo.h"
#include "Game.h"
#include "D3DWrapper.h"

void InitAnimations();

class PartAnimation
{
private:
	D3D::Matrix* start;
	D3D::Matrix* end;
	D3D::Matrix state;
	float amount;

public:
	PartAnimation(D3D::Matrix* start, D3D::Matrix* end)
	{
		this->start = start;
		this->end = end;

		this->Start();
	}

	void Start()
	{
		this->amount = 0;
	}

	void End()
	{
		this->amount = 1;
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
	std::vector<std::pair<Slot, PartAnimation*>> partAnimations;

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

		if (leftHeadlightAnim)
		{
			this->partAnimations.push_back({ Slot::LEFT_HEADLIGHT_GLASS, leftHeadlightAnim });
		}

		if (rightHeadlightAnim)
		{
			this->partAnimations.push_back({ Slot::RIGHT_HEADLIGHT_GLASS, rightHeadlightAnim });
		}
	}

	void Update()
	{
		if (Game::InFrontEnd())
		{
			auto leftHeadlight = this->GetAnimation(Slot::LEFT_HEADLIGHT);
			if (leftHeadlight)
			{
				leftHeadlight->End();
			}

			auto rightHeadlight = this->GetAnimation(Slot::RIGHT_HEADLIGHT);
			if (rightHeadlight)
			{
				rightHeadlight->End();
			}
		}

		if (Game::InRace())
		{
			auto leftHeadlight = this->GetAnimation(Slot::LEFT_HEADLIGHT);
			if (leftHeadlight)
			{
				leftHeadlight->End();
			}

			auto rightHeadlight = this->GetAnimation(Slot::RIGHT_HEADLIGHT);
			if (rightHeadlight)
			{
				rightHeadlight->End();
			}
		}
	}

	PartAnimation* GetAnimation(Slot slot)
	{
		for (auto anim : this->partAnimations)
		{
			if (anim.first == slot)
			{
				return anim.second;
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
					auto anim = new PartAnimation(start, end);
					this->partAnimations.push_back({ slot, anim });
					return anim;
				}
			}
		}

		return NULL;
	}

	void Clear()
	{
		for (auto& anim : this->partAnimations)
		{
			PartAnimation* ptr = anim.second;
			if (ptr)
			{
				for (auto& a : this->partAnimations)
				{
					if (a.second == ptr)
					{
						a.second = NULL;
					}
				}

				delete ptr;
			}
		}

		this->partAnimations.clear();
	}
};