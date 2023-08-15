#pragma once
#include "CarRenderInfo.h"
#include "Hashes.h"
#include "CarPartDatabase.h"

struct WheelTextureTable
{
	ReplacementTextureEntry TextureTable[5];

	void Init(Hash tire)
	{
		this->TextureTable[0].Set(Hashes::TIRE_STYLE01, Hashes::DEFAULTALPHA);
		this->TextureTable[1].Set(Hashes::TIRE_STYLE02, Hashes::DEFAULTALPHA);
		this->TextureTable[2].Set(Hashes::WHEEL_TIRE, tire);
		this->TextureTable[3].Set(Hashes::WHEEL_TIRE_N, StringHash1("_N", tire));
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

		auto rideInfo = this->carRenderInfo->RideInfo;

		auto tire = rideInfo->GetPart(Slot_Tires);

		auto frontWheel = rideInfo->GetPart(Slot::FRONT_WHEEL);
		auto rearWheel = rideInfo->GetPart(Slot::REAR_WHEEL);

		bool frontTire = !(frontWheel && frontWheel->GetAppliedAttributeBParam(Hashes::TIRE, true));
		bool rearTire = !(rearWheel && rearWheel->GetAppliedAttributeBParam(Hashes::TIRE, true));
		if (!tire && (frontTire || rearTire))
		{
			tire = CarPartDatabase::Instance->GetCarPart(Slot_Tires, CarType(-1), nullptr);
		}

		if (tire)
		{
			auto model = tire->GetAppliedAttributeIParam(Hashes::MODEL, 0);
			auto texture = tire->GetAppliedAttributeIParam(Hashes::TEXTURE_NAME, 0);
			if (model)
			{
				int radius = 0;

				if (frontWheel && (!frontWheel->IsStock() || frontTire))
				{
					radius = frontWheel->GetAppliedAttributeIParam(Hashes::INNER_RADIUS, 0);
					this->TextureTable[0].Init(texture);

					this->Tires[0].Init(FromIndex("_%d_A", radius, model));
					this->Tires[0].AttachReplacementTextureTable(this->TextureTable[0].TextureTable, 5);
				}

				if (rearWheel && (!rearWheel->IsStock() || rearWheel->GetAppliedAttributeBParam(Hashes::DEFAULT, false) || rearTire))
				{
					radius = rearWheel->GetAppliedAttributeIParam(Hashes::INNER_RADIUS, radius);
				}

				if (radius)
				{
					this->TextureTable[1].Init(texture);

					this->Tires[1].Init(FromIndex("_%d_A", radius, model));
					this->Tires[1].AttachReplacementTextureTable(this->TextureTable[1].TextureTable, 5);
				}
			}
		}

		this->UpdateBrands();
	}

	void AdjustWheelData()
	{
		this->AdjustWheelData(0);
		this->AdjustWheelData(1);
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

private:
	void AdjustWheelData(int num)
	{
		if (this->Tires[num].Solid)
		{
			D3DXVECTOR3 a, b;
			this->Tires[num].GetBoundingBox(&a, &b);

			float width = abs(a.y - b.y);
			float radius = abs(a.x - b.x) / 2.0f;
			this->carRenderInfo->WheelData.Widths[num] = width;
			this->carRenderInfo->WheelData.Radius[num] = radius;
		}
	}
};