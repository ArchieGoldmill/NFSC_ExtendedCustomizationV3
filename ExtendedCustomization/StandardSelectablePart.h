#pragma once
#include "DBCarPart.h"
#include "Slots.h"
#include "Hashes.h"
#include "Node.h"

class StandardSelectablePart
{
protected:
	int vTable;

public:
	Node<StandardSelectablePart*> Node;
	Hash HashName;
	DBCarPart* Part;
	char* Name;
	Slot SlotId;
};