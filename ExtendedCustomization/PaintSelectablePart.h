#pragma once
#include "StandardSelectablePart.h"
#include "ColorData.h"

struct PaintSelectablePart : StandardSelectablePart
{
	RecordColorData Color;
	//short unk; // padding
	DBCarPart* SecondPart;
};