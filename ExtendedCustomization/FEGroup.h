#pragma once
#include "FEObject.h"

struct FEGroup : FEObject
{
	FEMinList Children;
};