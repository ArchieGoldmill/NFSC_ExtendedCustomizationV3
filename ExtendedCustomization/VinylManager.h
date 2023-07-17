#pragma once
#include "Node.h"
#include "Hashes.h"
#include "D3DWrapper.h"

struct Vinyl : bNode<Vinyl>
{
	int NumPathSets;
	void* PathSets;
	Hash NameHash;
	bVector2 Center;
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
};