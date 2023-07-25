#pragma once
#include "Feature.h"
#include "FeCustomizeMain.h"

void __declspec(naked) WheelSizeCave1()
{
	static constexpr auto cExit = 0x0084020F;

	__asm
	{
		mov ecx, [esi + 0x34];
		mov eax, [ecx + 0x2C];
		cmp FeCustomizeMain::WheelSelected, 1;
		jne jExit;
		mov eax, 0x0B47AB2E;

	jExit:
		jmp cExit;
	}
}

void __declspec(naked) WheelSizeCave2()
{
	static constexpr auto Exit = 0x00840349;

	__asm
	{
		mov eax, [esi + 0x34];
		mov eax, [eax + 0x2C];
		cmp FeCustomizeMain::WheelSelected, 1;
		jne WheelSizeCave2_Exit;
		mov eax, 0x0B47AB2E;

	WheelSizeCave2_Exit:
		jmp Exit;
	}
}

void __declspec(naked) WheelSizeCave3()
{
	static constexpr auto Exit = 0x008469CC;

	__asm
	{
		mov edx, [esi + 0x34];
		mov eax, [ecx];
		mov edi, [edx + 0x2C];
		cmp FeCustomizeMain::WheelSelected, 1;
		jne WheelSizeCave3_Exit;
		mov edi, 0x0B47AB2E;

	WheelSizeCave3_Exit:
		jmp Exit;
	}
}

void __declspec(naked) WheelSizeCave4()
{
	static constexpr auto Exit = 0x0085FCE1;

	__asm
	{
		mov eax, [esp + 0x08];
		cmp FeCustomizeMain::WheelSelected, 1;
		jne WheelSizeCave4_Exit;
		mov eax, 0x0B47AB2E;

	WheelSizeCave4_Exit:
		cmp eax, 0xA234CD03;
		jmp Exit;
	}
}

void __declspec(naked) WheelSizeCave5()
{
	static constexpr auto Exit = 0x008663F3;

	__asm
	{
		cmp FeCustomizeMain::WheelSelected, 1;
		jne WheelSizeCave5_Exit;
		cmp eax, eax;
		jmp Exit;

	WheelSizeCave5_Exit:
		cmp[edi + 0x2C], 0x0B47AB2E;
		jmp Exit;
	}
}

void __declspec(naked) WheelSizeCave6()
{
	static constexpr auto Exit = 0x0086653C;

	__asm
	{
		mov eax, [esi + 0x34];
		mov edi, [eax + 0x2C];
		cmp FeCustomizeMain::WheelSelected, 1;
		jne WheelSizeCave6_Exit;
		mov edi, 0x0B47AB2E;

	WheelSizeCave6_Exit:
		jmp Exit;
	}
}

void InitWheelBrandsHelpBar()
{
	injector::MakeJMP(0x00840209, WheelSizeCave1);
	injector::MakeJMP(0x00840343, WheelSizeCave2);
	injector::MakeJMP(0x008469C4, WheelSizeCave3);
	injector::MakeJMP(0x0085FCD8, WheelSizeCave4);
	injector::MakeJMP(0x008663EC, WheelSizeCave5);
	injector::MakeJMP(0x00866536, WheelSizeCave6);
}