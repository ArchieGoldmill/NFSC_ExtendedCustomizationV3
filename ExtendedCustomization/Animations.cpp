#include "Animations.h"
#include "CarRenderInfoExtras.h"
#include "AutosculptSelectablePart.h"
#include "FrontEndRenderingCar.h"
#include "FeCustomizeParts.h"

void __stdcall RenderParts(CarRenderInfo* carRenderInfo, Slot slot, int view, eModel** model, D3DXMATRIX* marker, void* light, int flags)
{
	if (model && *model)
	{
		if (carRenderInfo->Extras->Animations)
		{
			auto animation = carRenderInfo->Extras->Animations->GetAnimation(slot);
			if (animation)
			{
				marker = animation->Get(marker);
			}
			else if (carRenderInfo->Extras->Animations->SlotNeedsMarker(slot))
			{
				return;
			}
		}

		(*model)->Render(view, marker, light, flags);
	}
}

void __fastcall RenderSpoiler(int view, int param, CarRenderInfo* carRenderInfo, eModel* model, D3DXMATRIX* marker, void* light, int data, int a1, int a2)
{
	eModel** modelPtr = &model;
	RenderParts(carRenderInfo, Slot::SPOILER, view, modelPtr, marker, light, data);
}

void ToggleAnimation(Slot slot)
{
	if (FEPartAnimation::LastSlot == slot)
	{
		// No need to update animation if slot does not change, prevents flickering of the animation
		return;
	}

	FEPartAnimation::LastSlot = slot;

	auto carRenderInfo = FrontEndRenderingCar::Get()->RideInfo.CarRenderInfo;
	if (carRenderInfo->Extras->Animations)
	{
		carRenderInfo->Extras->Animations->ResetAllAnimations();
		auto animate = g_Config.GetPart(slot, FrontEndRenderingCar::GetCarId()).Animation;
		if (animate == Animate::Headlights)
		{
			FEPartAnimation::Create(Slot::LEFT_HEADLIGHT);
			FEPartAnimation::Create(Slot::RIGHT_HEADLIGHT);
		}

		if (animate == Animate::LeftDoor || animate == Animate::Doors)
		{
			FEPartAnimation::Create(Slot::DOOR_LEFT);
		}

		if (animate == Animate::RightDoor || animate == Animate::Doors)
		{
			FEPartAnimation::Create(Slot::DOOR_RIGHT);
		}

		if (animate == Animate::Hood)
		{
			FEPartAnimation::Create(Slot::HOOD);
		}

		if (animate == Animate::Trunk)
		{
			FEPartAnimation::Create(Slot_Trunk);
		}
	}
}

int __fastcall StandardSelectablePart_Preview(StandardSelectablePart* _this, int, bool a)
{
	int result = _this->Preview(a);
	ToggleAnimation(_this->SlotId);
	return result;
}

void __fastcall sub_850BC0(FeCustomizeParts* _this, int, bool a)
{
	static auto _sub_850BC0 = (void(__thiscall*)(FeCustomizeParts*, bool))0x00850BC0;

	auto carRenderInfo = FrontEndRenderingCar::Get()->RideInfo.CarRenderInfo;
	if (carRenderInfo->Extras->Animations)
	{
		carRenderInfo->Extras->Animations->ResetAllAnimations();
	}

	_sub_850BC0(_this, a);
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
	if (g_Config.PartAnimations)
	{
		injector::MakeJMP(0x007DF272, RenderPartsHook);
		injector::MakeJMP(0x007DF5C9, RenderSpoilerHook);

		// Fix side mirrors
		injector::WriteMemory<char>(0x007E0DD2, 2);
		injector::WriteMemory<char>(0x007E0DDC, 2);

		injector::WriteMemory(0x009F9D00, StandardSelectablePart_Preview);
		injector::WriteMemory(0x009F78C4, sub_850BC0);
	}
}