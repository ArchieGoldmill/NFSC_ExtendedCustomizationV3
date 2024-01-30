#pragma once
#include "DBCarPart.h"
#include "Slots.h"
#include "Game.h"

struct CarPartDatabase
{
	static inline CarPartDatabase* Instance = (CarPartDatabase*)0x00B76690;

	DBCarPart* GetCarPart(Slot slot, CarType carId, DBCarPart* prevPart)
	{
		static auto _GetCarPart = (DBCarPart * (__thiscall*)(CarPartDatabase*, const char*, CarType, Slot, Hash, DBCarPart*, int))0x007CDDF0;
		return _GetCarPart(this, 0, carId, slot, 0, prevPart, -1);
	}

	DBCarPart* GetByKit(Slot slot, CarType carId, int kitNumber)
	{
		DBCarPart* part = 0;
		while (true)
		{
			part = this->GetCarPart(slot, carId, part);
			if (!part)
			{
				break;
			}

			int kit = part->GetKit();
			if (kit == kitNumber)
			{
				return part;
			}
		}

		return NULL;
	}

	DBCarPart* GetByKitW(Slot slot, CarType carId, int kitw)
	{
		DBCarPart* part = 0;
		while (true)
		{
			part = this->GetCarPart(slot, carId, part);
			if (!part)
			{
				break;
			}

			if (part->HasKitW(kitw))
			{
				return part;
			}
		}

		return NULL;
	}

	int GetPartNum(DBCarPart* partToSearch, Slot slot)
	{
		int num = 0;
		DBCarPart* part = 0;
		while (true)
		{
			num++;
			part = this->GetCarPart(slot, CarType(-1), part);
			if (!part || part == partToSearch)
			{
				break;
			}
		}

		return num;
	}

	DBCarPart* GetPartByNum(Slot slot, CarType carId, int num)
	{
		DBCarPart* part = NULL;
		do
		{
			--num;
			part = this->GetCarPart(slot, carId, part);
		} while (num > 0);

		return part;
	}

	DBCarPart* GetValidAttachment(Slot slot, RideInfo* rideInfo)
	{
		DBCarPart* part = NULL;
		while (true)
		{
			part = this->GetCarPart(slot, rideInfo->CarId, part);
			if (!part)
			{
				break;
			}

			if (part->HasMarkerName())
			{
				auto result = part->GetAttachMarker(rideInfo);
				if (result.first)
				{
					break;
				}
			}
			else
			{
				// if attachment does not have marker then it fits everything
				break;
			}
		}

		return part;
	}

	int GetNumCarParts(CarType carId, Slot slot)
	{
		FUNC(0x007D6660, int, __thiscall, _GetNumCarParts, CarPartDatabase*, CarType, Slot, Hash, int);
		return _GetNumCarParts(this, carId, slot, 0, -1);
	}
};