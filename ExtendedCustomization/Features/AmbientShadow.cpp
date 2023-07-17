#include "Feature.h"
#include "CarRenderInfo.h"
#include "CarRenderInfoExtras.h"
#include "Constants.h"

D3DCOLOR __stdcall GetShadowColor(CarRenderInfo* carRenderInfo, float brightness)
{
	bColor color;
	color.Color = 0x80808080;

	if (carRenderInfo->CarShadowTexture == carRenderInfo->Extras->Neon.NeonTexture)
	{
		color = carRenderInfo->Extras->Neon.GetColor();
	}

	if (carRenderInfo->CarShadowTexture == carRenderInfo->Extras->BrakelightGlow.Texture)
	{
		color = carRenderInfo->Extras->BrakelightGlow.GetColor();
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
float FrontShadowSize = 1.2f;
float RearShadowSize = 1.2f;
float SideShadowSize = 1.05f;
void InitNeon()
{
	injector::WriteMemory<float*>(0x007BE4F4, &CarDistMax, true);
	injector::WriteMemory<float*>(0x007BE50D, &CarDistMax, true);
	injector::WriteMemory<float*>(0x007BE501, &CarDistMult, true);
	injector::WriteMemory<float*>(0x007BE5B9, &FrontShadowSize, true);
	injector::WriteMemory<float*>(0x007BE5AB, &RearShadowSize, true);
	injector::WriteMemory<float*>(0x007BE59B, &SideShadowSize, true);
	injector::WriteMemory<float*>(0x007BE58B, &SideShadowSize, true);

	injector::MakeJMP(0x007BEA34, ShadowColorCave, true);
}