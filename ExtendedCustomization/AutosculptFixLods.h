#pragma once
#include "Feature.h"
#include "DBCarPart.h"
#include "RideInfo.h"
#include "AutoSculpt.h"

void __fastcall AutoSculptPose(AutoSculpt* autoSculpt, int, Hash* hashes, int count, RideInfo* rideInfo)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < count; j++)
		{
			if (hashes[j] != 0)
			{
				hashes[j]++;
			}
		}

		autoSculpt->Pose(hashes, count, rideInfo);
	}

	for (int j = 0; j < count; j++)
	{
		if (hashes[j] != 0)
		{
			hashes[j] -= 3;
		}
	}
	autoSculpt->Pose(hashes, count, rideInfo);
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

	return part->GetModelNameHash(a2, lod, autosculpt);
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