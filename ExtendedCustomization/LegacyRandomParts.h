#pragma once
#include "Feature.h"
#include "RideInfo.h"
#include "CarPartDatabase.h"

namespace Legacy
{
	DBCarPart* SetRandomPart(RideInfo* rideInfo, Slot slot, bool skipAutosculpt)
	{
		if (g_Config.GetPart(slot, rideInfo->CarId).State == State::Enabled)
		{
			auto carId = rideInfo->CarId;
			int numParts = CarPartDatabase::Instance->GetNumCarParts(carId, slot, skipAutosculpt);
			if (numParts)
			{
				int num = bRandom(numParts);
				auto part = CarPartDatabase::Instance->GetPartByNum(slot, carId, num);
				if (part)
				{
					rideInfo->SetPart(slot, part, true);
					return part;
				}
			}
		}

		return NULL;
	}

	void SetRandomParts(RideInfo* rideInfo, int hash)
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
					auto skirt = CarPartDatabase::Instance->GetByKitNumber(Slot::SKIRT, rideInfo->CarId, kit);
					if (skirt)
					{
						rideInfo->SetPart(Slot::SKIRT, skirt);
					}

					auto rearBumper = CarPartDatabase::Instance->GetByKitNumber(Slot::REAR_BUMPER, rideInfo->CarId, kit);
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

		HandleSpecialCustomizationV2(NULL, rideInfo, NULL, true);
	}
}