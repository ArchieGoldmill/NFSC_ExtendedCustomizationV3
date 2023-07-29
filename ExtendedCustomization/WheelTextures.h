#pragma once
#include "CarRenderInfo.h"
#include "Hashes.h"

struct WheelTextureTable
{
	ReplacementTextureEntry TextureTable[4];

	void Init(Hash tire)
	{
		this->TextureTable[0].Set(Hashes::TIRE_STYLE01, Hashes::DEFAULTALPHA);
		this->TextureTable[1].Set(Hashes::TIRE_STYLE02, Hashes::DEFAULTALPHA);
		this->TextureTable[2].Set(Hashes::WHEEL_TIRE, tire);
		this->TextureTable[3].Set(Hashes::WHEEL_TIRE_N, StringHash1("_N", tire));
	}
};

class CarWheelTextures
{
private:
	CarRenderInfo* carRenderInfo;

public:
	WheelTextureTable TextureTable[2];
	eModel Tires[2];

	CarWheelTextures(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
	}

	void Init()
	{
		auto rideInfo = this->carRenderInfo->RideInfo;
		auto tire = rideInfo->GetPart(Slot_Tires);
		if (tire)
		{
			auto model = tire->GetAppliedAttributeIParam(Hashes::MODEL, 0);
			auto texture = tire->GetAppliedAttributeIParam(Hashes::TEXTURE_NAME, 0);
			if (model)
			{
				auto frontWheel = rideInfo->GetPart(Slot::FRONT_WHEEL);
				int radius = 0;

				if (frontWheel && !frontWheel->IsStock())
				{
					radius = frontWheel->GetAppliedAttributeIParam(Hashes::INNER_RADIUS, 0);
					this->TextureTable[0].Init(texture);

					this->Tires[0].Init(FromIndex("_%d_A", radius, model));
					this->Tires[0].AttachReplacementTextureTable(this->TextureTable[0].TextureTable, 4);
				}

				auto rearWheel = rideInfo->GetPart(Slot::REAR_WHEEL);
				if (rearWheel && (!rearWheel->IsStock() || rearWheel->GetAppliedAttributeBParam(Hashes::DEFAULT, false)))
				{
					radius = rearWheel->GetAppliedAttributeIParam(Hashes::INNER_RADIUS, radius);
				}

				if (radius)
				{
					this->TextureTable[1].Init(texture);

					this->Tires[1].Init(FromIndex("_%d_A", radius, model));
					this->Tires[1].AttachReplacementTextureTable(this->TextureTable[1].TextureTable, 4);
				}
			}
		}
	}

	~CarWheelTextures()
	{
		for (auto& tire : this->Tires)
		{
			tire.UnInit();
		}
	}
};