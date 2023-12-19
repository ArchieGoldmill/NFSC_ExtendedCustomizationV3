#pragma once
#include "Hashes.h"
#include "Func.h"

struct eLightMaterial
{
	void* Vtable;
	eLightMaterial* Next;
	eLightMaterial* Prev;
	Hash NameHash;
	int Version;
	char Name[64];
	float DiffusePower;
	float DiffuseClamp;
	float DiffuseFlakes;
	float DiffuseVinylScale;
	float DiffuseMinLevel;
	float DiffuseMinRed;
	float DiffuseMinGreen;
	float DiffuseMinBlue;
	float DiffuseMaxLevel;
	float DiffuseMaxRed;
	float DiffuseMaxGreen;
	float DiffuseMaxBlue;
	float DiffuseMinAlpha;
	float DiffuseMaxAlpha;
	float SpecularPower;
	float SpecularFlakes;
	float SpecularVinylScale;
	float SpecularMinLevel;
	float SpecularMinRed;
	float SpecularMinGreen;
	float SpecularMinBlue;
	float SpecularMaxLevel;
	float SpecularMaxRed;
	float SpecularMaxGreen;
	float SpecularMaxBlue;
	float EnvmapPower;
	float EnvmapClamp;
	float EnvmapVinylScale;
	float EnvmapMinLevel;
	float EnvmapMinRed;
	float EnvmapMinGreen;
	float EnvmapMinBlue;
	float EnvmapMaxLevel;
	float EnvmapMaxRed;
	float EnvmapMaxGreen;
	float EnvmapMaxBlue;
	float VinylLuminanceMinLevel;
	float VinylLuminanceMaxLevel;
	int data1;

	static eLightMaterial* Get(Hash hash, int a)
	{
		FUNC(0x007473A0, eLightMaterial*, __cdecl, _get, Hash, int);
		return _get(hash, a);
	}
};