#pragma once
#include "CarRenderInfo.h"
#include "ColorData.h"

class CarPaint
{
private:
	CarRenderInfo* carRenderInfo;

	ColorMaterialData* CaliperMaterialData = NULL;
	ColorMaterialData* FrontLipMaterialData = NULL;
	ColorMaterialData* RearLipMaterialData = NULL;
	ColorMaterialData* RearRimMaterialData = NULL;
	ColorMaterialData* HoodMaterialData = NULL;
	ColorMaterialData* SpoilerMaterialData = NULL;

public:
	eModel* FrontWheel = NULL;
	eLightMaterial* FrontLipMaterial = NULL;
	eLightMaterial* RearLipMaterial = NULL;
	eLightMaterial* RearRimMaterial = NULL;

public:
	CarPaint(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
	}

	void Init()
	{
		this->InitPaint(this->CaliperMaterialData, this->carRenderInfo->Materials.Calipers, 0, Hashes::CALIPER);
		this->InitPaint(this->FrontLipMaterialData, this->FrontLipMaterial, 1, Hashes::MAGLIP);
		this->InitPaint(this->RearLipMaterialData, this->RearLipMaterial, 2, Hashes::MAGLIP);
		this->InitPaint(this->RearRimMaterialData, this->RearRimMaterial, 3, Hashes::MAGSILVER);
		if (!this->carRenderInfo->IsHoodCarbon)
		{
			this->InitPaint(this->HoodMaterialData, this->carRenderInfo->Materials.Hood, 4, 0);
		}
		this->InitPaint(this->SpoilerMaterialData, this->carRenderInfo->Materials.Spoiler, 5, 0);
	}

	~CarPaint()
	{
		if (this->CaliperMaterialData)
		{
			delete this->CaliperMaterialData;
			this->CaliperMaterialData = NULL;
		}

		if (this->FrontLipMaterialData)
		{
			delete this->FrontLipMaterialData;
			this->FrontLipMaterialData = NULL;
		}

		if (this->RearLipMaterialData)
		{
			delete this->RearLipMaterialData;
			this->RearLipMaterialData = NULL;
		}

		if (this->RearRimMaterialData)
		{
			delete this->RearRimMaterialData;
			this->RearRimMaterialData = NULL;
		}

		if (this->HoodMaterialData)
		{
			delete this->HoodMaterialData;
			this->HoodMaterialData = NULL;
		}

		if (this->SpoilerMaterialData)
		{
			delete this->SpoilerMaterialData;
			this->SpoilerMaterialData = NULL;
		}
	}

private:
	void InitPaint(ColorMaterialData*& materialData, eLightMaterial*& target, int num, Hash defaultMat)
	{
		auto paintData = this->carRenderInfo->RideInfo->GetCustomPaintData(num);
		if (paintData->CanGenerate())
		{
			if (!materialData)
			{
				materialData = new ColorMaterialData();
			}

			paintData->GenerateColorMaterialData(materialData, num);
			target = &materialData->LightMaterial;
			if (num == 4)
			{
				this->carRenderInfo->IsHoodCarbon = true;
			}
		}
		else
		{
			if (defaultMat)
			{
				target = eLightMaterial::Get(defaultMat, 0);
			}
		}
	}
};