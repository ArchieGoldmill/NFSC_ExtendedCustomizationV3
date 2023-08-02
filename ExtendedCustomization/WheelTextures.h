#pragma once
#include "CarRenderInfo.h"
#include "Hashes.h"

struct WheelTextureTable
{
	ReplacementTextureEntry TextureTable[5];

	void Init(Hash tire, int tireBrand)
	{
		this->TextureTable[0].Set(Hashes::TIRE_STYLE01, Hashes::DEFAULTALPHA);
		this->TextureTable[1].Set(Hashes::TIRE_STYLE02, Hashes::DEFAULTALPHA);
		this->TextureTable[2].Set(Hashes::WHEEL_TIRE, tire);
		this->TextureTable[3].Set(Hashes::WHEEL_TIRE_N, StringHash1("_N", tire));
		this->UpdateBrand(tireBrand);
	}

	void UpdateBrand(int tireBrand)
	{
		this->TextureTable[4].Set(Hashes::TIRE_BRAND, FromIndex("TIRE_BRAND_%02d", tireBrand));
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
					int tireBrand = rideInfo->AutoSculptRegions[ZoneStance].GetInt(6);
					this->TextureTable[0].Init(texture, tireBrand);

					this->Tires[0].Init(FromIndex("_%d_A", radius, model));
					this->Tires[0].AttachReplacementTextureTable(this->TextureTable[0].TextureTable, 5);
				}

				auto rearWheel = rideInfo->GetPart(Slot::REAR_WHEEL);
				if (rearWheel && (!rearWheel->IsStock() || rearWheel->GetAppliedAttributeBParam(Hashes::DEFAULT, false)))
				{
					radius = rearWheel->GetAppliedAttributeIParam(Hashes::INNER_RADIUS, radius);
				}

				if (radius)
				{
					int tireBrand = rideInfo->AutoSculptRegions[ZoneStance].GetInt(7);
					this->TextureTable[1].Init(texture, tireBrand);

					this->Tires[1].Init(FromIndex("_%d_A", radius, model));
					this->Tires[1].AttachReplacementTextureTable(this->TextureTable[1].TextureTable, 5);
				}
			}
		}
	}

	void UpdateBrands()
	{
		auto rideInfo = this->carRenderInfo->RideInfo;

		int tireBrand = rideInfo->AutoSculptRegions[ZoneStance].GetInt(6);
		this->TextureTable[0].UpdateBrand(tireBrand);

		tireBrand = rideInfo->AutoSculptRegions[ZoneStance].GetInt(7);
		this->TextureTable[1].UpdateBrand(tireBrand);
	}

	~CarWheelTextures()
	{
		for (auto& tire : this->Tires)
		{
			tire.UnInit();
		}
	}
};