#pragma once
#include <vector>
#include "Feature.h"
#include "Hashes.h"
#include "CarPartDatabase.h"
#include "WheelBrandsHelpBar.h"

struct WheelBrand
{
	Hash Brand;
	Hash Header;
};

inline std::vector<WheelBrand> WheelBrands;

bool WheelBrandsContains(Hash brand)
{
	for (auto& wheel : WheelBrands)
	{
		if (wheel.Brand == brand)
		{
			return true;
		}
	}

	return false;
}

Hash GetBrandByHeader(Hash header)
{
	for (auto& wheel : WheelBrands)
	{
		if (wheel.Header == header)
		{
			return wheel.Brand;
		}
	}

	return 0;
}

void GetWheelBrands()
{
	WheelBrand wheelBrand;
	wheelBrand.Brand = Hashes::STOCK;
	wheelBrand.Header = Hashes::COMMON_STOCK;
	WheelBrands.push_back(wheelBrand);

	DBCarPart* part = NULL;
	while (true)
	{
		part = CarPartDatabase::Instance->GetCarPart(Slot::FRONT_WHEEL, CarType(-1), part);
		if (part)
		{
			bool isas = part->IsAutosculpt();
			auto brand = part->GetAppliedAttributeIParam(Hashes::BRAND_NAME, 0);
			if (brand && !isas)
			{
				if (!WheelBrandsContains(brand))
				{
					wheelBrand.Brand = brand;
					wheelBrand.Header = StringHash1("_WHEEL_BRAND", brand);
					WheelBrands.push_back(wheelBrand);
				}
			}
		}
		else
		{
			break;
		}
	}
}

void __declspec(naked) PartListBrandCave()
{
	static constexpr auto Exit = 0x0085FA1E;

	__asm
	{
		mov ebp, [esp + 0x00000140];
		push ecx;

		cmp FeCustomizeMain::WheelSelected, 1;
		jne original;
		mov ebx, esp;
		add ebx, 0x140;
		mov ebx, [ebx];
		add ebx, 0x2C;
		mov ebx, [ebx];
		push ebx;

		xor ebx, ebx;
		jmp Exit;

	original:
		xor ebx, ebx;
		push ebx;
		jmp Exit;
	}
}

void InitWheelBrands()
{
	InitWheelBrandsHelpBar();

	injector::MakeJMP(0x0085FA13, PartListBrandCave);
}