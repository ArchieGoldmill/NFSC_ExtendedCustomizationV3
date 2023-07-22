#include "Feature.h"
#include "PaintSelectablePart.h"
#include "FECustomizationRecord.h"
#include "CarPartDatabase.h"
#include "FeCustomizeVisuals.h"
#include "Game.h"
#include "RimPaint.h"

void __fastcall PaintSelectablePart_Install(PaintSelectablePart* _this, int, FECustomizationRecord* record, int)
{
	if (record)
	{
		if (_this->SlotId == Slot::BODY_PAINT_SWATCH)
		{
			record->WriteColor(0, _this->Color, _this->Part, _this->SecondPart);
			return;
		}
		if (_this->SlotId == Slot::WHEEL_PAINT_SWATCH)
		{
			record->WriteColor(1, _this->Color, _this->Part, _this->SecondPart);
			return;
		}

		int num = -1;
		if (_this->SlotId == Slot::CALIPER_PAINT)
		{
			num = 0;
		}
		if (_this->SlotId == Slot::FRONT_LIP_PAINT)
		{
			num = 1;
		}
		if (_this->SlotId == Slot::REAR_LIP_PAINT)
		{
			num = 2;
		}
		if (_this->SlotId == Slot::REAR_RIM_PAINT)
		{
			num = 3;
		}
		if (_this->SlotId == Slot::HOOD_PAINT)
		{
			num = 4;
		}
		if (_this->SlotId == Slot::SPOILER_PAINT)
		{
			num = 5;
		}

		if (num != -1)
		{
			auto asColorData = record->GetASColorData(num);
			asColorData->Index1 = CarPartDatabase::Instance->GetPartNum(_this->Part, num > 3 ? Slot::BODY_PAINT_SWATCH : Slot::WHEEL_PAINT_SWATCH) / 100.0f;
			asColorData->Index2 = CarPartDatabase::Instance->GetPartNum(_this->SecondPart, num > 3 ? Slot::BODY_PAINT_GROUP : Slot::WHEEL_PAINT_GROUP) / 100.0f;
			asColorData->Brightness = _this->Color.Brightness / 100.0f;
			asColorData->Saturation = _this->Color.Saturation / 100.0f;
		}
	}
}

void __fastcall FECustomizationRecord_GetColor(FECustomizationRecord* record, int, int colorNum, ColorData* paintData)
{
	if (FeCustomizeVisuals::SelectedItem == VisualsMenu::BODY_PAINT || FeCustomizeVisuals::SelectedItem == VisualsMenu::FRONT_RIM_PAINT)
	{
		record->GetColor(colorNum, paintData);
	}
	else
	{
		int num = -1;
		if (FeCustomizeVisuals::SelectedItem == VisualsMenu::CALIPER_PAINT)
		{
			num = 0;
		}
		if (FeCustomizeVisuals::SelectedItem == VisualsMenu::FRONT_LIP_PAINT)
		{
			num = 1;
		}
		if (FeCustomizeVisuals::SelectedItem == VisualsMenu::REAR_LIP_PAINT)
		{
			num = 2;
		}
		if (FeCustomizeVisuals::SelectedItem == VisualsMenu::REAR_RIM_PAINT)
		{
			num = 3;
		}
		if (FeCustomizeVisuals::SelectedItem == VisualsMenu::HOOD_PAINT)
		{
			num = 4;
		}
		if (FeCustomizeVisuals::SelectedItem == VisualsMenu::SPOILER_PAINT)
		{
			num = 5;
		}

		if (num != -1)
		{
			auto ptr = record->GetASColorData(num);
			paintData->Brightness = ToInt(ptr->Brightness * 100.0f);
			paintData->Saturation = ToInt(ptr->Saturation * 100.0f);
			paintData->Part1 = CarPartDatabase::Instance->GetPartByNum(num > 3 ? Slot::BODY_PAINT_SWATCH : Slot::WHEEL_PAINT_SWATCH, ToInt(ptr->Index1 * 100.0f));
			paintData->Part2 = CarPartDatabase::Instance->GetPartByNum(num > 3 ? Slot::BODY_PAINT_GROUP : Slot::WHEEL_PAINT_GROUP, ToInt(ptr->Index2 * 100.0f));
		}
	}
}

void __stdcall GetPaintParts(PaintSelectablePart* part, Slot slot)
{
	part->Name = (char*)0x009F9CD8;
	switch (FeCustomizeVisuals::SelectedItem)
	{
	case VisualsMenu::CALIPER_PAINT: slot = Slot::CALIPER_PAINT; break;
	case VisualsMenu::FRONT_LIP_PAINT: slot = Slot::FRONT_LIP_PAINT; break;
	case VisualsMenu::REAR_LIP_PAINT: slot = Slot::REAR_LIP_PAINT; break;
	case VisualsMenu::REAR_RIM_PAINT: slot = Slot::REAR_RIM_PAINT; break;
	case VisualsMenu::HOOD_PAINT: slot = Slot::HOOD_PAINT; break;
	case VisualsMenu::SPOILER_PAINT: slot = Slot::SPOILER_PAINT; break;
	default:
		break;
	}

	part->SlotId = slot;
}

Hash __fastcall PaintSelectablePart_GetNameHash(PaintSelectablePart* _this)
{
	char* slotName = GetSlotName(_this->SlotId);
	if (!slotName)
	{
		if (_this->SlotId == Slot::CALIPER_PAINT)
		{
			slotName = (char*)"CALIPER_PAINT";
		}
		if (_this->SlotId == Slot::FRONT_LIP_PAINT)
		{
			slotName = (char*)"FRONT_LIP_PAINT";
		}
		if (_this->SlotId == Slot::REAR_LIP_PAINT)
		{
			slotName = (char*)"REAR_LIP_PAINT";
		}
		if (_this->SlotId == Slot::REAR_RIM_PAINT)
		{
			slotName = (char*)"REAR_RIM_PAINT";
		}
		if (_this->SlotId == Slot::HOOD_PAINT)
		{
			slotName = (char*)"HOOD_PAINT";
		}
		if (_this->SlotId == Slot::SPOILER_PAINT)
		{
			slotName = (char*)"SPOILER_PAINT";
		}
	}

	char buff[100];
	sprintf(buff, "CART_ITEM_NAME_%s", slotName);

	return StringHash(buff);
}

void __declspec(naked) GetPaintPartsCave()
{
	static constexpr auto cExit = 0x0085321F;
	__asm
	{
		pushad;
		push edi;
		push eax;
		call GetPaintParts;
		popad;
		jmp cExit;
	}
}

void InitPaints()
{
	if (g_Config.CustomPaints)
	{
		InitRimPaint();

		injector::WriteMemory(0x009F9E2C, PaintSelectablePart_Install);

		// Change part id for paint parts so they dont conflict in the cart
		injector::MakeJMP(0x00853215, GetPaintPartsCave);

		injector::WriteMemory(0x009F9E38, PaintSelectablePart_GetNameHash);

		injector::MakeCALL(0x00861C53, FECustomizationRecord_GetColor);
	}
}