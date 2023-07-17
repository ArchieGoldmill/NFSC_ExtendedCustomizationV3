#pragma once
#include "Feature.h"
#include "D3DWrapper.h"
#include "VinylManager.h"
#include <chrono>

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
		unpacked->TranslationX = Center.y - 1.0f - unpacked->TranslationX;

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
	scale.m[0][0] = packed->ScaleX * scaleFactor;
	scale.m[0][1] = 0;
	scale.m[1][0] = 0;
	scale.m[1][1] = packed->ScaleY * scaleFactor;

	unpacked->Matrix = scale * shear * rotation;
}

char __fastcall RasterizationManager_Initialize(char* _this, int, int a2, signed int canvasWidth, signed int canvasHeight, RideInfo* rideInfo, char vinylNum, char a7, int a8)
{
	static auto _Initialize = (char(__thiscall*)(void*, int, signed int, signed int, RideInfo*, char, char, int))0x007D09D0;
	auto res = _Initialize(_this, a2, canvasWidth, canvasHeight, rideInfo, vinylNum, a7, a8);

	if (!DecalMirror)
	{
		if (res && _this[0x5C] == 1)
		{
			auto vinylPart = rideInfo->GetVinylPart(vinylNum);
			if (vinylPart && vinylPart->GetAppliedAttributeIParam(Hashes::MIRROR, 0) == 0)
			{
				auto vinylManager = VinylManager::Get();
				auto node = vinylManager->LoadedVinyls.HeadNode.Next;
				auto nameHash = vinylPart->GetPartNameHash();
				while ((LPVOID)node != (LPVOID)vinylManager)
				{
					auto vinyl = (Vinyl*)node;
					if (vinyl->NameHash == nameHash)
					{
						Center = vinyl->Center;
						break;
					}

					node = node->Next;
				}

				_this[0x5C] = 0;
				DecalMirror = true;
			}
		}
	}
	else
	{
		_this[0x5C] = 0;
		DecalMirror = false;
	}

	return res;
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

void __declspec(naked) VinylLayerIncCave()
{
	static constexpr auto cExit = 0x007B2BCD;

	__asm
	{
		mov eax, [ecx + 0x38];
		mov dl, [ecx + 0x20];

		cmp DecalMirror, 1;
		je skipInc;
		inc eax;

	skipInc:
		jmp cExit;
	}
}

int* FeFastRep = (int*)0x005FDF66;
void __declspec(naked) FeColourChooserCtCave()
{
	__asm
	{
		mov ebx, [FeFastRep];
		mov dword ptr[ebx], -280;
		ret 0x10;
	}
}

void __declspec(naked) FeColourChooserDtCave()
{
	__asm
	{
		mov ebx, [FeFastRep];
		mov dword ptr[ebx], -200;
		ret;
	}
}

void* __fastcall FeVinylTransformCt(void* _this, int, void* menuScreen)
{
	*FeFastRep = -319;
	FUNC(0x0085C100, void*, __thiscall, _FeVinylTransformCt, void*, void*);
	return _FeVinylTransformCt(_this, menuScreen);
}

void __fastcall FeVinylTransformDt(void* _this)
{
	*FeFastRep = -200;
	FUNC(0x00841330, void*, __thiscall, _FeVinylTransformDt, void*);
	_FeVinylTransformDt(_this);
}

void InitVinyls()
{
	injector::MakeCALL(0x007C2EAB, GetVinylTransformUnpacked, true);

	injector::MakeCALL(0x007DBAAA, RasterizationManager_Initialize, true);
	injector::MakeCALL(0x007DBAE9, RasterizationManager_Initialize, true);

	injector::MakeJMP(0x007B2BC6, VinylLayerIncCave, true);

	injector::MakeCALL(0x00577A5F, MoveVinylUpDown, true);
	injector::MakeCALL(0x00577A72, MoveVinylLeftRight, true);

	// All vinyls mirrorable
	injector::MakeNOP(0x00841481, 2, true);
	injector::MakeNOP(0x00851E4F, 2, true);

	// Vinyl move speed
	injector::WriteMemory(FeFastRep, -200, true);
	injector::MakeJMP(0x008616D7, FeColourChooserCtCave, true);
	injector::MakeJMP(0x0085C343, FeColourChooserDtCave, true);
	injector::MakeCALL(0x00574F96, FeVinylTransformCt, true);
	injector::MakeCALL(0x00851BF3, FeVinylTransformDt, true);
}