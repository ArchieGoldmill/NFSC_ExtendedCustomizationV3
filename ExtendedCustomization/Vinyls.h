#pragma once
#include "Feature.h"
#include "D3DWrapper.h"

struct VinylPacked
{
	short TranslationX;
	short TranslationY;
	char Rotation;
	char Shear;
	char ScaleX;
	char ScaleY;
};

struct VinylUnpcacked
{
	bMatrix2 Matrix;
	float TranslationX;
	float TranslationY;
};

void __fastcall GetVinylTransformUnpacked(VinylPacked* packed, int, VinylUnpcacked* unpacked)
{
	float translation = 128.0f / 65535.0f;
	unpacked->TranslationX = packed->TranslationX * translation;
	unpacked->TranslationY = packed->TranslationY * translation;

	float shearFactor = 3.14f / 128.0f;
	bMatrix2 shear;
	shear.m[0][0] = 1;
	shear.m[0][1] = -packed->Shear * shearFactor;
	shear.m[1][0] = 0;
	shear.m[1][1] = 1;

	float angle = shearFactor * packed->Rotation;
	bMatrix2 rotation;
	rotation.m[0][0] = cos(angle);
	rotation.m[0][1] = sin(angle);
	rotation.m[1][0] = -sin(angle);
	rotation.m[1][1] = cos(angle);

	float scaleFactor = 0.0625;
	bMatrix2 scale;
	scale.m[0][0] = (packed->ScaleX + 16) * scaleFactor;
	scale.m[0][1] = 0;
	scale.m[1][0] = 0;
	scale.m[1][1] = (packed->ScaleY + 16) * scaleFactor;

	unpacked->Matrix = scale * shear * rotation;
}

void InitVinyls()
{
	injector::MakeCALL(0x007C2EAB, GetVinylTransformUnpacked, true);
}