#pragma once
#include <string>
#include "CarRenderInfo.h"
#include "DBCarPart.h"

class CarLicensePlateText
{
private:
	CarRenderInfo* carRenderInfo;

public:
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
			Hash plateHash = plate->GetAppliedAttributeIParam(Hashes::TEXTURE_NAME, 0);
			if (plate->IsAutosculpt())
			{
				int plateIndex = carRenderInfo->pRideInfo->Autosculpt.Regions[ZoneLicenseplate].GetInt(0);
				plateHash = StringHash1(std::to_string(plateIndex).c_str(), plateHash);
				this->carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::LicensePlate].Set(Hashes::LICENSEPLATE, plateHash);

				int frameIndex = carRenderInfo->pRideInfo->Autosculpt.Regions[ZoneLicenseplate].GetInt(1);
				Hash frameHash = FromIndex("LICENSEPLATE_FRAME%d", frameIndex);
				this->carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::LicensePlateFrame].Set(Hashes::LICENSEPLATE_FRAME, frameHash);

				for (int i = 0; i < 9; i++)
				{
					int textIndex = carRenderInfo->pRideInfo->Autosculpt.Regions[ZoneLicenseplate].GetInt(i + 2);

					Hash zoneHash = FromIndex("LICENSEPLATE_TEXT_ZONE%d", i);
					Hash zoneValue = FromIndex("LICENSEPLATE_TEXT%d", textIndex);
					this->carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::LicensePlate0 + i].Set(zoneHash, zoneValue);
				}
			}
			else
			{
				this->carRenderInfo->GeneralReplacementTextures[(int)ReplacementTextureIndex::LicensePlate].Set(Hashes::LICENSEPLATE, plateHash);
			}
		}
	}
};