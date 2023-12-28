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

struct FEPackage
{
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

	FEObject* FindObjectByHash(Hash hash)
	{
		static auto _FindObjectByHash = (FEObject * (__thiscall*)(FEPackage*, Hash))0x005F3760;
		return _FindObjectByHash(this, hash);
	}
};