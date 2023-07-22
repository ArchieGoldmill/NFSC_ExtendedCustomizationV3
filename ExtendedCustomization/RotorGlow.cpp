#include "CarRenderInfo.h"
#include "eModel.h"
#include "CarRenderInfoExtras.h"

void __fastcall RenderRotors(int view, int param, CarRenderInfo* carRenderInfo, int reflection, eModel* model, D3DXMATRIX* marker, D3DXMATRIX* light, int flags, int a1, int a2)
{
	model->Render(view, marker, light, flags);
	if (!reflection && carRenderInfo->Extras->RotorGlow)
	{
		carRenderInfo->Extras->RotorGlow->SetMarker(marker);
	}
}

void __declspec(naked) RenderRotorsHook()
{
	static constexpr auto cExit = 0x007E0C52;
	__asm
	{
		push[ebp + 0x28];
		push esi;
		call RenderRotors;

		jmp cExit;
	}
}

void InitRotorGlow()
{
	injector::MakeJMP(0x007E0C4D, RenderRotorsHook, true);
}