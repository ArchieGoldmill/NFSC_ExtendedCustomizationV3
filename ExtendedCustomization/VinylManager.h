#pragma once
#include "Node.h"
#include "Hashes.h"
#include "Math.h"

struct VinylPacked
{
	short TranslationX;
	short TranslationY;
	char Rotation;
	char Shear;
	char ScaleX;
	char ScaleY;
};

struct RIVinylColor
{
	DBCarPart* Hue;
	BYTE Saturation;
	BYTE Value;
};

struct RIVinyl
{
	DBCarPart* VinylPart;
	RIVinylColor Colors[4];
	VinylPacked Data;
};

struct Vinyl : bNode<Vinyl>
{
	int NumPathSets;
	void* PathSets;
	Hash NameHash;
	D3DXVECTOR2 Center;
	float AspectRatio;
};

class VinylManager
{
public:
	bList<Vinyl> LoadedVinyls;

	static VinylManager* Get()
	{
		return (VinylManager*)0x00B77010;
	}

	static Vinyl* GetByHash(Hash nameHash)
	{
		auto vinylManager = VinylManager::Get();
		auto node = vinylManager->LoadedVinyls.HeadNode.Next;
		while ((LPVOID)node != (LPVOID)vinylManager)
		{
			auto vinyl = (Vinyl*)node;
			if (vinyl->NameHash == nameHash)
			{
				return vinyl;
			}

			node = node->Next;
		}

		return NULL;
	}
};