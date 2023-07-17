#pragma once
#include "Feature.h"
#include "Hashes.h"
#include "Game.h"
#include "Config.h"

void MakeLod(char* str, char lod)
{
	int len = strlen(str);
	str[len - 1] = 'A';
}

int __cdecl ChangePartString(char* str, Hash model)
{
	if (g_Config.GetForceLodA(model) == State::Enabled)
	{
		MakeLod(str, 'A');
	}

	return StringHash1(str, model);
}

void __declspec(naked) HookPartLoadCave()
{
	static constexpr auto Exit = 0x007CDC8D;

	__asm
	{
		call ChangePartString;
		add esp, 0x8;
		jmp Exit;
	}
}

void InitForceLodA()
{
	injector::MakeJMP(0x007CDC85, HookPartLoadCave);
}