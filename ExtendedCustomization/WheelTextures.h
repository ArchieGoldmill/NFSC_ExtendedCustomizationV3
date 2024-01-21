#pragma once
#include "CarRenderInfo.h"
#include "Hashes.h"
#include "CarPartDatabase.h"

struct TireReplacementTable
{
	static const int Size = 5;
	ReplacementTextureEntry TextureTable[Size];

	void Init(Hash tire)
	{
		this->TextureTable[0].Set(Hashes::WHEEL_TIRE, tire);
		this->TextureTable[1].Set(Hashes::WHEEL_TIRE_N, StringHash1("_N", tire));
		this->TextureTable[3].Set(Hashes::TIRE_STYLE01, Hashes::DEFAULTALPHA);
		this->TextureTable[4].Set(Hashes::TIRE_STYLE02, Hashes::DEFAULTALPHA);
	}

	void UpdateBrand(int tireBrand)
	{
		this->TextureTable[2].Set(Hashes::TIRE_BRAND, FromIndex("TIRE_BRAND_%02d", tireBrand));
	}
};

struct CarTire
{
	ReplacementTextureEntry* TextureTable;
	eModel Model;

	void Init(Hash model, ReplacementTextureEntry* table)
	{
		this->TextureTable = table;
		this->Model.Init(model);
		this->Model.AttachReplacementTextureTable(this->TextureTable, TireReplacementTable::Size);
	}
};

class CarWheelTextures
{
private:	
	CarRenderInfo* carRenderInfo;
	TireReplacementTable tireReplacementTables[2];

public:
	CarTire Tires[4];

	CarWheelTextures(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;

		auto rideInfo = this->carRenderInfo->pRideInfo;

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

				if (frontWheel && ((!frontWheel->IsStock() && !frontWheel->IsAutosculpt()) || frontTire))
				{
					radius = frontWheel->GetAppliedAttributeIParam(Hashes::INNER_RADIUS, 0);
					this->tireReplacementTables[0].Init(texture);
					this->InitTire(WHEEL_FL, radius, model);
					this->InitTire(WHEEL_FR, radius, model);
				}

				if (rearWheel && ((!rearWheel->IsStock() && !rearWheel->IsAutosculpt()) || rearWheel->GetAppliedAttributeBParam(Hashes::DEFAULT, false) || rearTire))
				{
					radius = rearWheel->GetAppliedAttributeIParam(Hashes::INNER_RADIUS, radius);
				}

				if (radius)
				{
					this->tireReplacementTables[1].Init(texture);
					this->InitTire(WHEEL_RL, radius, model);
					this->InitTire(WHEEL_RR, radius, model);
				}
			}
		}

		this->UpdateBrands();
	}

	void AdjustWheelData()
	{
		this->AdjustWheelData(WHEEL_FL);
		this->AdjustWheelData(WHEEL_RL);
	}

	void UpdateBrands()
	{
		auto rideInfo = this->carRenderInfo->pRideInfo;

		int tireBrand = rideInfo->Autosculpt.Regions[ZoneStance].GetInt(6);
		this->tireReplacementTables[0].UpdateBrand(tireBrand);

		tireBrand = rideInfo->Autosculpt.Regions[ZoneStance].GetInt(7);
		this->tireReplacementTables[1].UpdateBrand(tireBrand);
	}

	~CarWheelTextures()
	{
		for (auto& wt : this->Tires)
		{
			wt.Model.UnInit();
		}
	}

private:

	void InitTire(int num, int radius, Hash model)
	{
		char buff[128];
		sprintf(buff, "_%s_%d_A", IsLeftWheel(num) ? "LEFT" : "RIGHT", radius);

		this->Tires[num].Init(StringHash1(buff, model), this->tireReplacementTables[IsRearWheel(num)].TextureTable);
	}

	void AdjustWheelData(int num)
	{
		if (this->Tires[num].Model.Solid)
		{
			D3DXVECTOR3 a, b;
			this->Tires[num].Model.GetBoundingBox(&a, &b);

			float width = abs(a.y - b.y);
			float radius = abs(a.x - b.x) / 2.0f;

			int side = num < 2 ? 0 : 1;
			this->carRenderInfo->WheelData.Widths[side] = width;
			this->carRenderInfo->WheelData.Radius[side] = radius;
		}
	}
};