#include "Animations.h"
#include "CarRenderInfoExtras.h"

void __stdcall RenderParts(CarRenderInfo* carRenderInfo, Slot slot, int view, eModel** model, D3DXMATRIX* marker, void* light, int flags)
{
	if (model && *model)
	{
		auto animation = carRenderInfo->Extras->Animations.GetAnimation(slot);
		if (animation)
		{
			marker = animation->Get(marker);
		}
		else if (carRenderInfo->Extras->Animations.SlotNeedsMarker(slot))
		{
			return;
		}

		(*model)->Render(view, marker, light, flags);
	}
}

void __fastcall RenderSpoiler(int view, int param, CarRenderInfo* carRenderInfo, eModel* model, D3DXMATRIX* marker, void* light, int data, int a1, int a2)
{
	eModel** modelPtr = &model;
	RenderParts(carRenderInfo, Slot::SPOILER, view, modelPtr, marker, light, data);
}

void __declspec(naked) RenderSpoilerHook()
{
	static constexpr auto cExit = 0x007DF5CE;
	__asm
	{
		push esi;
		call RenderSpoiler;

		jmp cExit;
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
	injector::MakeJMP(0x007DF272, RenderPartsHook);
	injector::MakeJMP(0x007DF5C9, RenderSpoilerHook);

	// Fix side mirrors
	injector::WriteMemory<char>(0x007E0DD2, 2);
	injector::WriteMemory<char>(0x007E0DDC, 2);
}