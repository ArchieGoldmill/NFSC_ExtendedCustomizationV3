#pragma once
#include <vector>
#include "Feature.h"
#include "Slots.h"
#include "Hashes.h"
#include "FECarRecord.h"
#include "Config.h"
#include "CarPartDatabase.h"
#include "Game.h"
#include "FrontEndRenderingCar.h"
#include "AutosculptSelectablePart.h"
#include "CarCustomizeManager.h"
#include "FECustomizationRecord.h"

Slot KitwParts[] = {
	Slot::HOOD, Slot::FRONT_BUMPER, Slot::REAR_BUMPER, Slot::SKIRT, Slot_FrontFender, Slot_RearFender, Slot_Trunk, Slot::LEFT_HEADLIGHT, Slot::LEFT_BRAKELIGHT
};

template<typename T>
bool Contains(T* ar, T s, int size)
{
	size = size / sizeof(T);
	for (int i = 0; i < size; i++)
	{
		if (ar[i] == s)
		{
			return true;
		}
	}

	return false;
}

void AddNodeToList(bNode<StandardSelectablePart*>* listHead, bNode<StandardSelectablePart*>* slot)
{
	auto listEnd = listHead->Prev;
	listEnd->Next = slot;
	listHead->Prev = slot;
	slot->Prev = listEnd;
	slot->Next = listHead;
}

bool CheckKitwPart(Slot slot, DBCarPart* part)
{
	auto rideInfo = &(FrontEndRenderingCar::Get()->RideInfo);

	auto bodyPart = rideInfo->GetPart(Slot::BODY);
	if (bodyPart)
	{
		auto kit = bodyPart->GetAppliedAttributeIParam(Hashes::KITNUMBER, 0);
		return part->HasKitW(kit);
	}

	return true;
}

bool CheckMarker(Slot slot, DBCarPart* part)
{
	if (part->HasMarkerName())
	{
		for (auto attachSlot : AttachSlots)
		{
			if (attachSlot == slot)
			{
				auto rideInfo = &(FrontEndRenderingCar::Get()->RideInfo);
				auto marker = part->GetAttachMarker(rideInfo);

				return marker == NULL;
			}
		}
	}

	return false;
}

void HandleCart(AutosculptSelectablePart* selectPart, Slot slot)
{
	float* zones = 0;

	auto cart = CarCustomizeManager::Get()->IsPartTypeInCart(slot);
	if (cart)
	{
		auto cartSelectPart = (AutosculptSelectablePart*)cart[3];
		if (cartSelectPart)
		{
			auto cartPart = cartSelectPart->Part;
			if (cartPart && cartPart->IsAutosculpt())
			{
				if (selectPart->Part == cartPart)
				{
					cartSelectPart->SetToCurrentPart();
					int* param = (int*)0x00BBB050;
					if (*param)
					{
						static auto sub_8433D0 = (float* (__thiscall*)(int))0x008433D0;
						zones = sub_8433D0(*param);
					}
				}
			}
		}
	}
	else
	{
		auto part = CarCustomizeManager::Get()->GetInstalledPart(slot);
		if (part && !part->IsStock() && part == selectPart->Part)
		{
			static auto sub_49C860 = (float* (__thiscall*)(FECustomizationRecord*, int))0x0049C860;
			zones = sub_49C860(FECustomizationRecord::Get(), selectPart->Region);
		}
	}

	selectPart->Zones = zones;
}

template<typename SelectablePart>
void GetPartsListV3(Slot slot, bNode<SelectablePart*>* listHead, bool isCarbon, Hash brandName, int innerRadius, CarType carId, bool isAutosculpt)
{
	DBCarPart* part = NULL;
	while (true)
	{
		part = CarPartDatabase::Instance->GetCarPart(slot, carId, part);
		if (!part)
		{
			break;
		}

		if (!part->IsStock())
		{
			bool as = part->IsAutosculpt();
			if ((isAutosculpt && !as) || (!isAutosculpt && as))
			{
				continue;
			}

			if (isCarbon != part->IsCarbon())
			{
				continue;
			}
		}

		if (Contains(KitwParts, slot, sizeof(KitwParts)) && !CheckKitwPart(slot, part))
		{
			continue;
		}

		if (CheckMarker(slot, part))
		{
			continue;
		}

		auto selectablePart = (SelectablePart*)j_malloc_0(sizeof(SelectablePart));
		*selectablePart = SelectablePart(slot, part);
		if (isAutosculpt)
		{
			HandleCart((AutosculptSelectablePart*)selectablePart, slot);
		}

		AddNodeToList((bNode<StandardSelectablePart*>*)listHead, (bNode<StandardSelectablePart*>*)&selectablePart->NodeItem);
	}
}

void __cdecl StandardSelectablePart_GetPartsList(Slot slot, bNode<StandardSelectablePart*>* listHead, bool isCarbon, Hash brandName, int innerRadius)
{
	auto carId = FECarRecord::GetCarType();
	int version = g_Config.GetVersion(carId);
	if (version == 3 && slot != Slot::FRONT_WHEEL)
	{
		GetPartsListV3<StandardSelectablePart>(slot, listHead, isCarbon, brandName, innerRadius, carId, false);
	}
	else
	{
		StandardSelectablePart::GetPartsList(slot, listHead, isCarbon, brandName, innerRadius);
	}
}

void __cdecl AutosculptSelectablePart_GetPartsList(Slot slot, bNode<AutosculptSelectablePart*>* listHead, bool isCarbon, int brandName, int innerRadius)
{
	auto carId = FECarRecord::GetCarType();
	int version = g_Config.GetVersion(carId);
	if (version == 3)
	{
		GetPartsListV3<AutosculptSelectablePart>(slot, listHead, isCarbon, brandName, innerRadius, carId, true);
	}
	else
	{
		AutosculptSelectablePart::GetPartsList(slot, listHead, isCarbon, brandName, innerRadius);
	}
}

void InitPartList()
{
	injector::MakeCALL(0x0085FA37, StandardSelectablePart_GetPartsList, true);
	injector::MakeCALL(0x0085FA30, AutosculptSelectablePart_GetPartsList, true);
}