#pragma once
#include "CarRenderInfo.h"
#include "Hashes.h"
#include "CarPartDatabase.h"

struct CarTire
{
	static const int Size = 3;

	eModel Model;
	ReplacementTextureEntry TextureTable[Size];

	void Init(Hash model, Hash tire)
	{
		this->TextureTable[0].Set(Hashes::WHEEL_TIRE, tire);
		this->TextureTable[1].Set(Hashes::WHEEL_TIRE_N, StringHash1("_N", tire));
		this->Model.Init(model);
		this->Model.AttachReplacementTextureTable(this->TextureTable, Size);
	}

	void UpdateBrand(int tireBrand)
	{
		this->TextureTable[2].Set(Hashes::TIRE_BRAND, FromIndex("TIRE_BRAND_%02d", tireBrand));
	}
};

struct CarWheel
{
	static const int Size = 2;

	ReplacementTextureEntry TextureTable[Size];

	void Init()
	{
		// Remove vanilla tires
		this->TextureTable[0].Set(Hashes::TIRE_STYLE01, Hashes::DEFAULTALPHA);
		this->TextureTable[1].Set(Hashes::TIRE_STYLE02, Hashes::DEFAULTALPHA);
	}
};

struct CarWheelTire
{
	CarTire Tire;
	CarWheel Wheel;
};

class CarWheelTextures
{
private:
	CarRenderInfo* carRenderInfo;

public:
	CarWheelTire WheelTires[4];

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
					this->InitTire(WHEEL_FL, radius, model, texture);
					this->InitTire(WHEEL_FR, radius, model, texture);
				}

				if (rearWheel && ((!rearWheel->IsStock() && !rearWheel->IsAutosculpt()) || rearWheel->GetAppliedAttributeBParam(Hashes::DEFAULT, false) || rearTire))
				{
					radius = rearWheel->GetAppliedAttributeIParam(Hashes::INNER_RADIUS, radius);
				}

				if (radius)
				{
					this->InitTire(WHEEL_RL, radius, model, texture);
					this->InitTire(WHEEL_RR, radius, model, texture);
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
		this->WheelTires[WHEEL_FL].Tire.UpdateBrand(tireBrand);
		this->WheelTires[WHEEL_FR].Tire.UpdateBrand(tireBrand);

		tireBrand = rideInfo->Autosculpt.Regions[ZoneStance].GetInt(7);
		this->WheelTires[WHEEL_RL].Tire.UpdateBrand(tireBrand);
		this->WheelTires[WHEEL_RR].Tire.UpdateBrand(tireBrand);
	}

	~CarWheelTextures()
	{
		for (auto& wt : this->WheelTires)
		{
			wt.Tire.Model.UnInit();
		}
	}

private:

	void InitTire(int num, int radius, Hash model, Hash texture)
	{
		this->WheelTires[num].Tire.Init(FromIndex("_%d_A", radius, model), texture);
		this->WheelTires[num].Wheel.Init();
	}

	void AdjustWheelData(int num)
	{
		if (this->WheelTires[num].Tire.Model.Solid)
		{
			D3DXVECTOR3 a, b;
			this->WheelTires[num].Tire.Model.GetBoundingBox(&a, &b);

			float width = abs(a.y - b.y);
			float radius = abs(a.x - b.x) / 2.0f;

			int side = num < 2 ? 0 : 1;
			this->carRenderInfo->WheelData.Widths[side] = width;
			this->carRenderInfo->WheelData.Radius[side] = radius;
		}
	}
};