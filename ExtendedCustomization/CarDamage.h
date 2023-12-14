#pragma once

enum DamageZone : unsigned int
{
	DamageZone_None = 0,
	DamageZone_Front = 0x7,				// 0000 0000 0000 0000 0000 0111
	DamageZone_Rear = 0x38,				// 0000 0000 0000 0000 0011 1000
	DamageZone_FrontLeft = 0x7000,		// 0000 0000 0111 0000 0000 0000
	DamageZone_FrontRight = 0x38000,	// 0000 0011 1000 0000 0000 0000
	DamageZone_RearLeft = 0x1C0000,		// 0001 1100 0000 0000 0000 0000
	DamageZone_RearRight = 0xE00000		// 1110 0000 0000 0000 0000 0000
};

enum DamageType
{
	DamageType_Undamaged = 0,
	DamageType_Light = 1,
	DamageType_Medium = 2,
	DamageType_Heavy = 4
};

struct CarDamage
{
	DamageZone Damage;

	bool IsDamaged(DamageZone zone)
	{
		return ((unsigned int)this->Damage & (unsigned int)zone) != 0;
	}

	bool IsDamaged(DamageZone zone, int type)
	{
		return this->IsDamaged(zone, (DamageType)type);
	}

	bool IsDamaged(DamageZone zone, DamageType type)
	{
		unsigned int damage = (unsigned int)this->Damage;
		if (zone == DamageZone_Rear)
		{
			damage = damage >> 3;
		}
		if (zone == DamageZone_FrontLeft)
		{
			damage = damage >> 12;
		}
		if (zone == DamageZone_FrontRight)
		{
			damage = damage >> 15;
		}
		if (zone == DamageZone_RearLeft)
		{
			damage = damage >> 18;
		}
		if (zone == DamageZone_RearRight)
		{
			damage = damage >> 21;
		}

		return (damage & (unsigned int)type) != 0;
	}

	bool IsHeadlightLeftDamaged()
	{
		return this->IsDamaged(DamageZone_Front) && this->IsDamaged(DamageZone_FrontLeft, DamageType_Medium | DamageType_Heavy);
	}

	bool IsHeadlightRightDamaged()
	{
		return this->IsDamaged(DamageZone_Front) && this->IsDamaged(DamageZone_FrontRight, DamageType_Medium | DamageType_Heavy);
	}

	bool IsBrakelightLeftDamaged()
	{
		return this->IsDamaged(DamageZone_Rear) && this->IsDamaged(DamageZone_RearLeft, DamageType_Medium | DamageType_Heavy);
	}
	
	bool IsBrakelightRightDamaged()
	{
		return this->IsDamaged(DamageZone_Rear) && this->IsDamaged(DamageZone_RearRight, DamageType_Medium | DamageType_Heavy);
	}
};