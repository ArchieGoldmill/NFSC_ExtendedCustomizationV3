#pragma once
#include "FEObject.h"

struct FEPoint
{
	float H;
	float V;
};

struct FEObjectMouseState
{
	FEObject* Object;
	FEPoint Offset;
	int Flags;
};

class FEPackage
{
public:
	void* Vtable;
	FEObject* Next;
	FEObject* Prev;
	char* Name;
	int NameHash;
	int _data[12];
	FEMinList Objects;
	int _data1[22];
	FEObjectMouseState* MouseObjectStates;
	int NumMouseObjects;
};