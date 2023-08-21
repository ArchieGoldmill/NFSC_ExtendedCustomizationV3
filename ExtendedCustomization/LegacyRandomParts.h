#pragma once
#include "Feature.h"
#include "RideInfo.h"
#include "CarPartDatabase.h"

DBCarPart* SetRandomPart(RideInfo* rideInfo, Slot slot, bool skipAutosculpt, int kitw = -1)
{
	if (g_Config.GetPart(slot, rideInfo->CarId).State == State::Enabled)
	{
		auto carId = rideInfo->CarId;
		auto carName = StringHash(GetCarTypeName(carId));
		DBCarPart* part = null;
		std::vector< DBCarPart*> parts;
		if (kitw >= 0)
		{
			auto kitwPart = CarPartDatabase::Instance->GetByKitW(slot, carId, kitw);
			if (!kitwPart)
			{
				kitw = 0;
			}
		}

		while (true)
		{
			part = CarPartDatabase::Instance->GetCarPart(slot, carId, part);
			if (!part)
			{
				break;
			}

			if (skipAutosculpt && part->IsAutosculpt())
			{
				continue;
			}

			if (kitw >= 0 && !part->HasKitW(kitw))
			{
				continue;
			}

			if (slot == Slot::VINYL_GENERIC)
			{
				auto carVinyl = part->GetAppliedAttributeIParam(Hashes::CAR_NAME, 0);
				if (carVinyl != carName)
				{
					continue;
				}
			}

			parts.push_back(part);
		}

		int numParts = parts.size();
		if (numParts)
		{
			int num = bRandom(numParts);
			part = parts[num];
			rideInfo->SetPart(slot, part, true);
			return part;
		}
	}

	return null;
}

void SetRandomNeon(RideInfo* rideInfo)
{
	if (bRandom(2))
	{
		auto neon = SetRandomPart(rideInfo, Slot_Neon, false);
		if (neon)
		{
			rideInfo->AutoSculptRegions[ZoneNeon].Zones[0] = (float)bRandom(99) / 100.0f;
			rideInfo->AutoSculptRegions[ZoneNeon].Zones[2] = 0.5f;
			auto num = neon->GetAppliedAttributeIParam(Hashes::MORPHTARGET_NUM, 0);
			if (num == 4)
			{
				rideInfo->AutoSculptRegions[ZoneNeon].Zones[2] = (float)bRandom(99) / 100.0f;
			}
		}
	}
}

void SetRandomDecals(RideInfo* rideInfo)
{
	if (bRandom(2))
	{
		SetRandomPart(rideInfo, Slot::DECAL_FRONT_WINDOW_TEX0, false);
		SetRandomPart(rideInfo, Slot::DECAL_REAR_WINDOW_TEX0, false);
	}
}

void SetRandomVinyl(RideInfo* rideInfo)
{
	auto vinyl = rideInfo->GetPart(Slot::VINYL_GENERIC);
	if (!vinyl || vinyl->IsStock())
	{
		if (bRandom(2))
		{
			SetRandomPart(rideInfo, Slot::VINYL_GENERIC, false);
		}
	}
}

void SetRandomTires(RideInfo* rideInfo)
{
	SetRandomPart(rideInfo, Slot_Tires, false);
	if (bRandom(2))
	{
		float tire = (float)bRandom(10) / 100.0f;
		rideInfo->AutoSculptRegions[ZoneStance].Zones[6] = tire;
		rideInfo->AutoSculptRegions[ZoneStance].Zones[7] = tire;
	}
}

namespace Legacy
{
	void SetRandomPartsV2(RideInfo* rideInfo, int hash)
	{
		auto body = SetRandomPart(rideInfo, Slot::BODY, false);
		if (body && body->IsStock())
		{
			SetRandomPart(rideInfo, Slot::FRONT_BUMPER_BADGING_SET, false);
			SetRandomPart(rideInfo, Slot::REAR_BUMPER_BADGING_SET, false);
			auto frontBumper = SetRandomPart(rideInfo, Slot::FRONT_BUMPER, false);
			if (frontBumper)
			{
				int kit = frontBumper->GetKit();
				if (kit)
				{
					auto skirt = CarPartDatabase::Instance->GetByKit(Slot::SKIRT, rideInfo->CarId, kit);
					if (skirt)
					{
						rideInfo->SetPart(Slot::SKIRT, skirt);
					}

					auto rearBumper = CarPartDatabase::Instance->GetByKit(Slot::REAR_BUMPER, rideInfo->CarId, kit);
					if (rearBumper)
					{
						rideInfo->SetPart(Slot::REAR_BUMPER, rearBumper);
					}
				}
			}
		}

		SetRandomPart(rideInfo, Slot::SPOILER, true);
		SetRandomPart(rideInfo, Slot::HOOD, true);

		for (int i = (int)Slot::ATTACHMENT0; i <= (int)Slot::ATTACHMENT15; i++)
		{
			SetRandomPart(rideInfo, (Slot)i, false);
		}

		SetRandomPart(rideInfo, Slot::STEERINGWHEEL, false);

		SetRandomVinyl(rideInfo);
		SetRandomNeon(rideInfo);
		SetRandomDecals(rideInfo);
		SetRandomTires(rideInfo);

		HandleSpecialCustomizationV2(NULL, rideInfo, NULL, true);
	}

	void SetRandomParts(RideInfo* rideInfo, int hash)
	{
		rideInfo->SetRandomParts(hash);

		SetRandomVinyl(rideInfo);
		SetRandomNeon(rideInfo);
		SetRandomDecals(rideInfo);
		SetRandomTires(rideInfo);
	}
}