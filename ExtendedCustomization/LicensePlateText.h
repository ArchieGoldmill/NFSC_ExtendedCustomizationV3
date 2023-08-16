#pragma once
#include <string>
#include "CarRenderInfo.h"
#include "DBCarPart.h"

class CarLicensePlateText
{
private:
	CarRenderInfo* carRenderInfo;

public:
	ReplacementTextureEntry TextureTable[11];

	CarLicensePlateText(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
	}

	void Init()
	{
		this->UpdateLicensePlate();
	}

	void UpdateLicensePlate()
	{
		auto plate = this->carRenderInfo->pRideInfo->GetPart(Slot::LICENSE_PLATE);
		if (plate)
		{
			if (plate->IsAutosculpt())
			{
				Hash plateh = plate->GetAppliedAttributeIParam(Hashes::TEXTURE_NAME, 0);
				int plateIndex = carRenderInfo->pRideInfo->AutoSculptRegions[ZoneLicenseplate].GetInt(0);
				Hash plateHash = StringHash1(std::to_string(plateIndex).c_str(), plateh);
				this->TextureTable[0].Set(Hashes::LICENSEPLATE, plateHash);

				int frameIndex = carRenderInfo->pRideInfo->AutoSculptRegions[ZoneLicenseplate].GetInt(1);
				Hash frameHash = FromIndex("LICENSEPLATE_FRAME%d", frameIndex);
				this->TextureTable[1].Set(Hashes::LICENSEPLATE_FRAME, frameHash);

				for (int i = 0; i < 9; i++)
				{
					int textIndex = carRenderInfo->pRideInfo->AutoSculptRegions[ZoneLicenseplate].GetInt(i + 2);

					Hash zoneHash = FromIndex("LICENSEPLATE_TEXT_ZONE%d", i);
					Hash zoneValue = FromIndex("LICENSEPLATE_TEXT%d", textIndex);

					this->TextureTable[i + 2].Set(zoneHash, zoneValue);
				}
			}
		}
	}
};