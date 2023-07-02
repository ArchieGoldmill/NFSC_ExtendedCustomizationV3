#pragma once
#include "DBCarPart.h"
#include "Slots.h"
#include "Hashes.h"
#include "Node.h"

class StandardSelectablePart
{
public:
	Node<StandardSelectablePart*> Node;
	Hash HashName;
	DBCarPart* Part;
	char* Name;
	Slot SlotId;

	virtual ~StandardSelectablePart();
	virtual int CartComparisonValue();
};