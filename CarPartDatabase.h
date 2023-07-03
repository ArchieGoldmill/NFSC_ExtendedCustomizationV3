#pragma once
#include "DBCarPart.h"
#include "Slots.h"
#include "Game.h"

class CarPartDatabase
{
public:
	static CarPartDatabase* Get()
	{
		return (CarPartDatabase*)0x00B76690;
	}

	DBCarPart* GetCarPart(Slot slot, CarType carId, DBCarPart* prevPart)
	{
		static auto _GetCarPart = (DBCarPart * (__thiscall*)(CarPartDatabase*, const char*, CarType, Slot, Hash, DBCarPart*, int))0x007CDDF0;
		return _GetCarPart(this, 0, carId, slot, 0, prevPart, -1);
	}

	DBCarPart* GetByKitNumber(Slot slot, CarType carId, int kitNumber)
	{
		DBCarPart* part = 0;
		while (true)
		{
			part = this->GetCarPart(slot, carId, part);
			if (!part)
			{
				break;
			}

			int kit = part->GetAppliedAttributeIParam(Hashes::KITNUMBER, 0);
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
};