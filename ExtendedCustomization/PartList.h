#pragma once
#include "Feature.h"
#include "Slots.h"
#include "Hashes.h"

void __cdecl StandardSelectablePart_GetPartsList(Slot slot, Node<StandardSelectablePart*>* listHead, bool isCarbon, Hash brandName, int innerRadius)
{

}

void __cdecl AutosculptSelectablePart_GetPartsList(Slot slot, Node<StandardSelectablePart*>* listHead, bool isCarbon, int brandName, int innerRadius)
{

}

void InitPartList()
{
	injector::MakeCALL(0x0085FA37, StandardSelectablePart_GetPartsList, true);
	injector::MakeCALL(0x0085FA30, AutosculptSelectablePart_GetPartsList, true);
}