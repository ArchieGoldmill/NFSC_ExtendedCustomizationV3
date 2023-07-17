#pragma once
#include "Feature.h"
#include "D3DWrapper.h"
#include "VinylManager.h"
#include "SmoothVinyls.h"
#include "RasterizationManager.h"
#include "CarSkinManager.h"

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

bool DecalMirror = false;
bVector2 Center;
void __fastcall GetVinylTransformUnpacked(VinylPacked* packed, int, VinylUnpcacked* unpacked)
{
	float translationFactor = 2.0f / 65535.0f;
	unpacked->TranslationX = packed->TranslationX * translationFactor;
	unpacked->TranslationY = packed->TranslationY * translationFactor;

	if (DecalMirror)
	{
		VinylPacked mirrored;
		mirrored = *packed;

		mirrored.Rotation = 127 - mirrored.Rotation;
		unpacked->TranslationX = (1.0f - Center.x) * 2 - 1.0f - unpacked->TranslationX;

		packed = &mirrored;
	}

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

char __fastcall RasterizationManager_Initialize(RasterizationManager* rasterizationManager, int, TextureInfo* texture, signed int canvasWidth, signed int canvasHeight, RideInfo* rideInfo, char vinylNum, char a7, int a8)
{
	auto result = rasterizationManager->Initialize(texture, canvasWidth, canvasHeight, rideInfo, vinylNum, a7, a8);

	if (!DecalMirror)
	{
		if (result && rasterizationManager->Mirror)
		{
			auto vinylPart = rideInfo->GetVinylPart(vinylNum);
			if (vinylPart && !vinylPart->GetAppliedAttributeIParam(Hashes::MIRROR, 0))
			{
				auto vinyl = VinylManager::GetByHash(vinylPart->GetPartNameHash());
				if (vinyl)
				{
					Center = vinyl->Center;
					rasterizationManager->Mirror = false;
					DecalMirror = true;
				}
			}
		}
	}
	else
	{
		rasterizationManager->Mirror = false;
		DecalMirror = false;
	}

	return result;
}

void MoveVinyl(short& val, int dir)
{
	float speed = 60;
	if (GetKeyState(VK_SHIFT) & 0x8000)
	{
		speed *= 2;
	}
	if (GetKeyState(VK_CONTROL) & 0x8000)
	{
		speed /= 2;
	}

	val += (float)dir * speed;

	if (val > 32767)
	{
		val = 32767;
	}

	if (val < -32768)
	{
		val = -32768;
	}
}

void __fastcall MoveVinylUpDown(VinylPacked* vinyl, int, int dir)
{
	MoveVinyl(vinyl->TranslationX, dir);
}

void __fastcall MoveVinylLeftRight(VinylPacked* vinyl, int, int dir)
{
	MoveVinyl(vinyl->TranslationY, dir);
}

void __fastcall CarSkinManagerFinalize(CarSkinManager* skinManager)
{
	if (DecalMirror)
	{
		if (skinManager->RasterizeLowerLayers)
		{
			skinManager->CurrentTask = skinManager->CurrentLayer < skinManager->NumLowerLayers ? eCST_LowerLayersInitialize : eCST_CanvasInitialize;
		}
		else
		{
			skinManager->CurrentTask = eCST_CanvasInitialize;
		}
	}
	else
	{
		skinManager->CurrentLayer++;
		if (skinManager->RasterizeLowerLayers)
		{
			if (skinManager->CurrentLayer == skinManager->NumLowerLayers)
			{
				skinManager->CurrentTask = skinManager->unk_2[5] ? eCST_CompressLowerLayers : eCST_CopyLowerLayersToCanvas;
				return;
			}
			else if (skinManager->CurrentLayer < skinManager->NumLowerLayers)
			{
				skinManager->CurrentTask = eCST_LowerLayersInitialize;
				return;
			}
		}

		if (skinManager->CurrentLayer == skinManager->NumLayers)
		{
			skinManager->CurrentTask = skinManager->unk_3[1] ? eCST_CopyToDestinationNoCompression : eCST_CopyToDestination;
		}
		else
		{
			skinManager->CurrentTask = eCST_CanvasInitialize;
		}
	}
}

void __declspec(naked) CarSkinManagerFinalizeCave()
{
	__asm
	{
		call CarSkinManagerFinalize;
		ret;
	}
}

void InitVinyls()
{
	InitSmoothVinyls();

	injector::MakeCALL(0x007C2EAB, GetVinylTransformUnpacked);

	injector::MakeCALL(0x007DBAAA, RasterizationManager_Initialize);
	injector::MakeCALL(0x007DBAE9, RasterizationManager_Initialize);

	injector::MakeJMP(0x007B2BC6, CarSkinManagerFinalizeCave);

	injector::MakeCALL(0x00577A5F, MoveVinylUpDown);
	injector::MakeCALL(0x00577A72, MoveVinylLeftRight);

	// All vinyls mirrorable
	injector::MakeNOP(0x00841481, 2);
	injector::MakeNOP(0x00851E4F, 2);
}