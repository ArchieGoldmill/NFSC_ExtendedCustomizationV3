#include "Animations.h"
#include "CarRenderInfoExtras.h"
#include "AutosculptSelectablePart.h"
#include "FrontEndRenderingCar.h"
#include "FeCustomizeParts.h"

void CalcTrunkAnim(CarRenderInfo* carRenderInfo, D3DXMATRIX* partMarker, IPartMarker* markerData, D3DXMATRIX* marker)
{
	auto animMarker = ((PartAnimation*)markerData)->Calculate();
	D3DXMatrixMultiply(marker, partMarker, &animMarker);

	auto position = carRenderInfo->GetMatrix();
	D3DXMatrixMultiply(marker, marker, position);
}

void __stdcall RenderParts(CarRenderInfo* carRenderInfo, Slot slot, int view, eModel** model, D3DXMATRIX* marker, void* light, int flags)
{
	if (model && *model)
	{
		if (slot == Slot::UNIVERSAL_SPOILER_BASE)
		{
			auto spoiler = carRenderInfo->RideInfo->GetPart(Slot::SPOILER);
			if (spoiler && spoiler->GetAppliedAttributeBParam(Hashes::USEMARKER2, false))
			{
				return;
			}
		}

		if (carRenderInfo->Extras->Animations)
		{
			auto animation = carRenderInfo->Extras->Animations->GetAnimation(slot);
			if (animation)
			{
				bool handled = false;
				if (slot == Slot::SPOILER)
				{
					auto spoiler = carRenderInfo->RideInfo->GetPart(slot);
					if (spoiler && spoiler->GetUpgradeLevel() > 0)
					{
						CalcTrunkAnim(carRenderInfo, &carRenderInfo->Markers.Spoiler->Matrix, animation, marker);
						handled = true;
					}
				}

				if (slot == Slot::LICENSE_PLATE)
				{
					D3DXMATRIX rot;
					D3DXMatrixRotationY(&rot, 3.14f / 2);
					D3DXMatrixMultiply(&rot, &rot, &carRenderInfo->Markers.RearLicensePlate->Matrix);
					CalcTrunkAnim(carRenderInfo, &rot, animation, marker);
					handled = true;
				}

				if (!handled)
				{
					marker = animation->Get(marker);
				}
			}
			else if (carRenderInfo->Extras->Animations->SlotNeedsMarker(slot))
			{
				// Do not render attachment if marker is not found
				return;
			}
		}

		(*model)->Render(view, marker, light, flags);
	}
}

void __fastcall RenderSpoiler(int view, int, CarRenderInfo* carRenderInfo, eModel* model, D3DXMATRIX* marker, void* light, int data, int a1, int a2)
{
	auto spoiler = carRenderInfo->RideInfo->GetPart(Slot::SPOILER);
	if (spoiler && spoiler->GetUpgradeLevel() > 0)
	{
		if (!carRenderInfo->Markers.Spoiler)
		{
			return;
		}
	}

	eModel** pModel = &model;
	RenderParts(carRenderInfo, Slot::SPOILER, view, pModel, marker, light, data);
}

void __fastcall RenderRoofScoop(int view, int, CarRenderInfo* carRenderInfo, eModel* model, D3DXMATRIX* marker, void* light, int data, int a1, int a2)
{
	if (!carRenderInfo->Markers.RoofScoop)
	{
		return;
	}

	eModel** pModel = &model;
	RenderParts(carRenderInfo, Slot::ROOFSCOOP, view, pModel, marker, light, data);
}

void __fastcall RenderLicensePlate(int view, int, CarRenderInfo* carRenderInfo, int type, eModel* model, D3DXMATRIX* marker, void* light, int data, int a1, int a2)
{
	if (type == 1)
	{
		eModel** modelPtr = &model;
		RenderParts(carRenderInfo, Slot::LICENSE_PLATE, view, modelPtr, marker, light, data);
	}
	else
	{
		model->Render(view, marker, light, data);
	}
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

void __declspec(naked) RenderRoofScoopHook()
{
	static constexpr auto cExit = 0x007DF6D1;
	__asm
	{
		push esi;
		call RenderRoofScoop;

		jmp cExit;
	}
}

void __declspec(naked) RenderLicensePlateHook()
{
	static constexpr auto cExit = 0x007DF490;
	__asm
	{
		push[esp + 0x34];
		push esi;
		call RenderLicensePlate;

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
		injector::MakeJMP(0x007DF6CC, RenderRoofScoopHook);
		injector::MakeJMP(0x007DF48B, RenderLicensePlateHook);

		// Fix side mirrors
		injector::WriteMemory<char>(0x007E0DD2, 2);
		injector::WriteMemory<char>(0x007E0DDC, 2);

		injector::WriteMemory(0x009F9D00, StandardSelectablePart_Preview);
		injector::WriteMemory(0x009F78C4, sub_850BC0);
	}
}