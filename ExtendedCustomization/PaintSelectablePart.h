#pragma once
#include "StandardSelectablePart.h"
#include "ColorData.h"

struct PaintSelectablePart : StandardSelectablePart
{
	RecordColorData Color;
	DBCarPart* SecondPart;
};