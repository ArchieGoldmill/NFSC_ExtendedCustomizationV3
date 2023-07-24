#pragma once
#include <string>
#include "CarRenderInfo.h"
#include "DBCarPart.h"

class CarTextures
{
private:
	CarRenderInfo* carRenderInfo;

public:
	ReplacementTextureEntry LicensePlate[11];

	CarTextures(CarRenderInfo* carRenderInfo)
	{
		this->carRenderInfo = carRenderInfo;
	}

	void Init()
	{
		this->UpdateLicensePlate();
	}

	void UpdateLicensePlate()
	{
		auto plate = this->carRenderInfo->RideInfo->GetPart(Slot::LICENSE_PLATE);
		if (plate)
		{
			if (plate->IsAutosculpt())
			{
				Hash plateh = plate->GetAppliedAttributeIParam(Hashes::TEXTURE_NAME, 0);
				int plateIndex = carRenderInfo->RideInfo->AutoSculptRegions[ZoneLicenseplate].GetInt(0);
				Hash plateHash = StringHash1(std::to_string(plateIndex).c_str(), plateh);
				this->LicensePlate[0].Set(Hashes::LICENSEPLATE, plateHash);

				int frameIndex = carRenderInfo->RideInfo->AutoSculptRegions[ZoneLicenseplate].GetInt(1);
				Hash frameHash = FromIndex("LICENSEPLATE_FRAME%d", frameIndex);
				this->LicensePlate[1].Set(Hashes::LICENSEPLATE_FRAME, frameHash);

				for (int i = 0; i < 9; i++)
				{
					int textIndex = carRenderInfo->RideInfo->AutoSculptRegions[ZoneLicenseplate].GetInt(i + 2);

					Hash zoneHash = FromIndex("LICENSEPLATE_TEXT_ZONE%d", i);
					Hash zoneValue = FromIndex("LICENSEPLATE_TEXT%d", textIndex);

					this->LicensePlate[i + 2].Set(zoneHash, zoneValue);
				}
			}
		}
	}
};