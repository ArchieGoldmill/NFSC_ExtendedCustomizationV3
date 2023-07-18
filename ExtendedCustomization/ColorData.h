#pragma once
#include "eLightMaterial.h"

struct DBCarPart;

struct RecordColorData
{
	char Brightness;
	char Saturation;
};

struct ColorData :RecordColorData
{
	DBCarPart* Part1;
	DBCarPart* Part2;
};

struct ColorMaterialData
{
	DBCarPart* Part1;
	DBCarPart* Part2;
	float Brightness;
	float Saturation;
	eLightMaterial LightMaterial;

	bool GenerateColorMaterialData(DBCarPart* part1, DBCarPart* part2, float b, float s)
	{
		FUNC(0x007D2E60, bool, __thiscall, _Generate, ColorMaterialData*, DBCarPart*, DBCarPart*, float, float);
		return _Generate(this, part1, part2, b, s);
	}
};

struct ASColorData
{
	float Index1;
	float Index2;
	float Brightness;
	float Saturation;

	bool CanGenerate()
	{
		return this->Index1 && this->Index2;
	}

	void GenerateColorMaterialData(ColorMaterialData* data, int num);
};