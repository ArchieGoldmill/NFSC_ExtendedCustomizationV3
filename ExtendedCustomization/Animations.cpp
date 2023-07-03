#include "Animations.h"
#include "CarRenderInfoExtras.h"

void __stdcall RenderParts(CarRenderInfo* carRenderInfo, Slot slot, int view, eModel** model, D3D::Matrix* marker, void* light, int flags)
{
	if (model && *model)
	{
		auto animation = carRenderInfo->Extras->Animations.GetAnimation(slot);
		if (animation)
		{
			marker = animation->Get(marker);
		}

		(*model)->Render(view, marker, light, flags);
	}
}

void __declspec(naked) RenderPartsHook()
{
	static constexpr auto cExit = 0x007DF277;
	__asm
	{
		push[esp + 0x28];
		push ecx;
		call RenderParts;

		jmp cExit;
	}
}

void InitAnimations()
{
	injector::MakeJMP(0x007DF272, RenderPartsHook, true);
}