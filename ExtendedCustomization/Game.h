#pragma once
#include "Hashes.h"
#include "Func.h"
#include "TextureInfo.h"

INLINE_FUNC(0x00471080, Hash, __cdecl, bStringHash, const char*, Hash);
INLINE_FUNC(0x00471050, Hash, __cdecl, StringHash, const char*);
INLINE_FUNC(0x0055CFD0, TextureInfo*, __cdecl, GetTextureInfo, Hash, bool, bool);

#define SAVE_REGS_ESI __asm\
{\
	__asm push eax\
	__asm push ebx\
	__asm push ecx\
	__asm push edx\
	__asm push edi\
}\

#define RESTORE_REGS_ESI __asm\
{\
	__asm pop edi\
	__asm pop edx\
	__asm pop ecx\
	__asm pop ebx\
	__asm pop eax\
}\

struct CarType
{
	int Id;
};

namespace Game
{
	static float* CarBrightness = (float*)0x009EA968;
	static float* DeltaTime = (float*)0x00A99A5C;

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