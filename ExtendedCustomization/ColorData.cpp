#include "ColorData.h"
#include "CarPartDatabase.h"
#include "Func.h"

void ASColorData::GenerateColorMaterialData(ColorMaterialData* data, int num)
{
	if (this->CanGenerate())
	{
		auto part1 = CarPartDatabase::Instance->GetPartByNum(num > 3 ? Slot::BODY_PAINT_SWATCH : Slot::WHEEL_PAINT_SWATCH, CarType(-1), ToInt(this->Index1 * 100.0f));
		auto part2 = CarPartDatabase::Instance->GetPartByNum(num > 3 ? Slot::BODY_PAINT_GROUP : Slot::WHEEL_PAINT_GROUP, CarType(-1), ToInt(this->Index2 * 100.0f));
		float val1 = 1.0f - ToInt(this->Brightness * 100.0f) * 0.071428575f;
		float val2 = 1.0f - ToInt(this->Saturation * 100.0f) * 0.11111111f;
		data->GenerateColorMaterialData(part2, part1, val1, val2);
	}
}