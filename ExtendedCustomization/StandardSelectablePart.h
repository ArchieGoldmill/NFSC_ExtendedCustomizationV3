#pragma once
#include "DBCarPart.h"
#include "Slots.h"
#include "Hashes.h"
#include "Node.h"
#include "FECustomizationRecord.h"

struct StandardSelectablePart
{
	int vTable;
	bNode<StandardSelectablePart*> NodeItem;
	Hash HashName;
	DBCarPart* Part;
	char* Name;
	Slot SlotId;

	StandardSelectablePart(Slot slot, DBCarPart* part)
	{
		this->vTable = 0x009F9CF0;
		this->Part = part;
		this->SlotId = slot;
		this->HashName = 0x881F8EFA;
		this->Name = (char*)0x009F9CD8;
	}

	bool Install(FECustomizationRecord* record, bool setOnly)
	{
		FUNC(0x008420F0, bool, __thiscall, _Install, StandardSelectablePart*, FECustomizationRecord*, bool);
		return _Install(this, record, setOnly);
	}

	Hash GetCategoryHash()
	{
		static auto _GetCategoryHash = (Hash(__thiscall*)(StandardSelectablePart*))0x00842670;
		return _GetCategoryHash(this);
	}

	static void GetPartsList(Slot slot, bNode<StandardSelectablePart*>* listHead, bool isCarbon, Hash brandName, int innerRadius)
	{
		FUNC(0x00852940, void, __cdecl, _GetPartsList, Slot, bNode<StandardSelectablePart*>*, bool, Hash, int);
		_GetPartsList(slot, listHead, isCarbon, brandName, innerRadius);
	}
};