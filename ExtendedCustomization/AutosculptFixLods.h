#pragma once
#include "Feature.h"
#include "DBCarPart.h"

void __fastcall AutoSculptPose(void* _this, int param, Hash* hashes, int a3, int a4)
{
	static auto _AutoSculptPose = (void(__thiscall*)(void*, Hash*, int, int))0x007DC7F0;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < a3; j++)
		{
			if (hashes[j] != 0)
			{
				hashes[j]++;
			}
		}

		_AutoSculptPose(_this, hashes, a3, a4);
	}

	for (int j = 0; j < a3; j++)
	{
		if (hashes[j] != 0)
		{
			hashes[j] -= 3;
		}
	}
	_AutoSculptPose(_this, hashes, a3, a4);
}

Hash __cdecl ArnGetModelNameHash2(DBCarPart* part, int a2, int lod, int autosculpt)
{
	if (autosculpt > 11)
	{
		lod = 1;
	}

	if (autosculpt > 22)
	{
		lod = 2;
	}

	if (autosculpt > 33)
	{
		lod = 3;
	}

	autosculpt = autosculpt % 11;
	if (autosculpt == 0)
	{
		autosculpt = 11;
	}

	static auto _GetModelNameHash = (Hash(__cdecl*)(DBCarPart*, int, int, int))0x007CDA40;
	return _GetModelNameHash(part, a2, lod, autosculpt);
}


void InitAutosculptFixLods()
{
	if (g_Config.FixAutosculptLods)
	{
		injector::MakeCALL(0x007E4D44, AutoSculptPose);
		injector::WriteMemory<char>(0x007CFDA4, 45);
		injector::MakeCALL(0x007CFD5D, ArnGetModelNameHash2);
	}
}