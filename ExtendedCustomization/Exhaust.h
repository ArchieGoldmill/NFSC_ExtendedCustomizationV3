#pragma once
#include "Feature.h"

void __declspec(naked) NosEffectCave()
{
	static constexpr auto cExit = 0x007D5F7C;

	__asm
	{
		mov al, [ecx + 0xB5];
		mov ebp, 0xE3031005; // fxcar_shift
		test al, al;
		je NosEffectCave_Exit;
		mov ebp, 0x6B7916BD; // fxcar_nos

	NosEffectCave_Exit:
		jmp cExit;
	}
}

void InitExhaust()
{
	injector::MakeJMP(0x007D5F76, NosEffectCave);
}