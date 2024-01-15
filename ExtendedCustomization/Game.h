#pragma once
#include <cstdio>
#include "Hashes.h"
#include "Func.h"
#include "TextureInfo.h"
#include "Slots.h"

struct CarType
{
	int Id;
};

#define WHEEL_FL 0
#define WHEEL_FR 1
#define WHEEL_RR 2
#define WHEEL_RL 3

inline bool IsFrontWheel(int wheel)
{
	return wheel == WHEEL_FL || wheel == WHEEL_FR;
}

inline bool IsRearWheel(int wheel)
{
	return wheel == WHEEL_RL || wheel == WHEEL_RR;
}

INLINE_FUNC(0x00471080, Hash, __cdecl, StringHash1, const char*, Hash);
INLINE_FUNC(0x00471050, Hash, __cdecl, StringHash, const char*);
INLINE_FUNC(0x00402560, Hash, __cdecl, StringHash32, const char*);
INLINE_FUNC(0x006A1560, void*, __cdecl, j_malloc_0, int);
INLINE_FUNC(0x007B0290, char*, __cdecl, GetCarTypeName, CarType);
INLINE_FUNC(0x0046E580, void, __cdecl, HSV2RGB, float, float, float, float*, float*, float*);
INLINE_FUNC(0x00841BD0, void, __cdecl, RGB2HSV, unsigned char, unsigned char, unsigned char, float*, float*, float*);
INLINE_FUNC(0x0046DB30, int, __cdecl, bRandom, int);
INLINE_FUNC(0x007B1410, char*, __cdecl, GetSlotName, Slot);
INLINE_FUNC(0x00477BE0, void*, __cdecl, bOMalloc, int);
INLINE_FUNC(0x004A62E0, bool, __cdecl, IsPaused);
INLINE_FUNC(0x004B65F0, bool, __thiscall, DALVehicle_GetRPM, void* DALVehicle, float* getVal, const int playerNum);
INLINE_FUNC(0x004B6690, bool, __thiscall, DALVehicle_GetRedLine, void* DALVehicle, float* getVal, const int playerNum);
INLINE_FUNC(0x006B5980, void, __cdecl, LoadResourceFile, const char* path, int type, int, int, int, int, int);
INLINE_FUNC(0x00402800, void*, __cdecl, eFrameMalloc, int size);
INLINE_FUNC(0x00463A80, int*, __thiscall, Attrib_GetAttributePointer, int, int, int);
INLINE_FUNC(0x005A0910, void, __cdecl, FE_String_Printf, char*, Hash, const char*, ...);

inline void HSV2RGB_255(float h, float s, float v, float* r, float* g, float* b)
{
	HSV2RGB(h, s, v, r, g, b);
	*r *= 255.0f;
	*g *= 255.0f;
	*b *= 255.0f;
}

inline Hash FromIndex(const char* str, int index)
{
	char buff[128];
	sprintf(buff, str, index);
	return StringHash(buff);
}

inline Hash FromIndex(const char* str, int index, Hash hash)
{
	char buff[128];
	sprintf(buff, str, index);
	return StringHash1(buff, hash);
}

namespace Game
{
	inline float DeltaTime()
	{
		return *(float*)0x00A99A5C;
	}

	static auto CarPartSlotMap = (int*)0x00A73398;
	static auto ScreenSizeX = (int*)0x00A63F80;
	static auto ScreenSizeY = (int*)0x00A63F84;
	static auto CopyPoseValueToFamilyMap = (int*)0x00A79FC8;
	static auto FrontSteerAngle = (float*)0x00A7B668;
	static auto CarEmitterPositionSlotPool = (int*)0x00B74C34;
	static auto AutosculptRegionList = (Slot*)0x00A79F70;
	static auto ForceHeadlightsOff = (bool*)0x00B74C18;
	static auto FeFastRep = (int*)0x005FDF66;
	static auto FePhotoModeMenuScreen_Instance = (int*)0x00A97B10;
	static auto NotInFocus = (bool*)0x00AB0B3C;

	enum class State
	{
		NONE = 0,
		LOADING_FRONTEND = 1,
		UNLOADING_FRONTEND = 2,
		IN_FRONTEND = 3,
		LOADING_REGION = 4,
		LOADING_TRACK = 5,
		RACING = 6,
		UNLOADING_TRACK = 7,
		UNLOADING_REGION = 8,
		EXIT_DEMO_DISC = 9
	};

	inline bool InState(State state)
	{
		return *(State*)0x00A99BBC == state;
	}

	inline bool InFrontEnd()
	{
		return InState(Game::State::IN_FRONTEND);
	}

	inline bool InRace()
	{
		return InState(Game::State::RACING);
	}
}