#include "Feature.h"
#include "CarRenderInfo.h"
#include "CarRenderInfoExtras.h"
#include "Constants.h"

D3DCOLOR __stdcall GetShadowColor(CarRenderInfo* carRenderInfo, float brightness)
{
	Color color(0x80808080);

	if (carRenderInfo->Extras->Neon && carRenderInfo->CarShadowTexture == carRenderInfo->Extras->Neon->NeonTexture)
	{
		color = carRenderInfo->Extras->Neon->GetColor();
	}

	if (carRenderInfo->Extras->BrakelightGlow && carRenderInfo->CarShadowTexture == carRenderInfo->Extras->BrakelightGlow->Texture)
	{
		color = carRenderInfo->Extras->BrakelightGlow->GetColor();
	}

	color.Bytes[3] *= brightness;
	return color.Color;
}

void __declspec(naked) ShadowColorCave()
{
	static constexpr auto hExit = 0x007BEA6D;

	__asm
	{
		push eax;
		fstp[esp];
		push ebx;
		call GetShadowColor;

		mov ecx, 1;
		mov ebx, [ebx + 0x3F8];
		jmp hExit;
	}
}

float CarDistMax = 2.0f;
float CarDistMult = CarDistMax / 4.0f;
float FrontShadowSize = 1.15f;
float RearShadowSize = 1.15f;
float SideShadowSize = 1.15f;
void InitNeon()
{
	if (g_Config.Neon)
	{
		injector::WriteMemory<float*>(0x007BE4F4, &CarDistMax);
		injector::WriteMemory<float*>(0x007BE50D, &CarDistMax);
		injector::WriteMemory<float*>(0x007BE501, &CarDistMult);
		injector::WriteMemory<float*>(0x007BE5B9, &FrontShadowSize);
		injector::WriteMemory<float*>(0x007BE5AB, &RearShadowSize);
		injector::WriteMemory<float*>(0x007BE59B, &SideShadowSize);
		injector::WriteMemory<float*>(0x007BE58B, &SideShadowSize);
	}

	if (g_Config.Neon || g_Config.BrakelightGlow)
	{
		injector::MakeJMP(0x007BEA34, ShadowColorCave);
	}

	if (g_Config.IgnoreSpoilerBoundingBox)
	{
		injector::WriteMemory<BYTE>(0x007D592C + (int)Slot::SPOILER, 1);
		injector::WriteMemory<BYTE>(0x007D592C + (int)Slot::LEFT_SIDE_MIRROR, 1);
		injector::WriteMemory<BYTE>(0x007D592C + (int)Slot::RIGHT_SIDE_MIRROR, 1);
		injector::WriteMemory<BYTE>(0x007D592C + (int)Slot::FRONT_BUMPER_BADGING_SET, 0);
		injector::WriteMemory<BYTE>(0x007D592C + (int)Slot::REAR_BUMPER_BADGING_SET, 0);
		injector::WriteMemory<BYTE>(0x007D592C + (int)Slot::FRONT_BUMPER, 0);
		injector::WriteMemory<BYTE>(0x007D592C + (int)Slot::REAR_BUMPER, 0);
		injector::WriteMemory<BYTE>(0x007D592C + (int)Slot::SKIRT, 0);
	}
}