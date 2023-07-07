#pragma once
#include "StandardSelectablePart.h"

class AutosculptSelectablePart : public StandardSelectablePart
{
public:
	int Region;
	float* Zones;
};