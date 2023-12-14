#pragma once

enum class DamageZone : unsigned int
{
	Front = 0x7,			// 0000 0000 0000 0000 0000 0111
	Rear = 0x38,			// 0000 0000 0000 0000 0011 1000
	FrontLeft = 0x7000,		// 0000 0000 0111 0000 0000 0000
	FrontRight = 0x38000,	// 0000 0011 1000 0000 0000 0000
	RearLeft = 0x1C0000,	// 0001 1100 0000 0000 0000 0000
	RearRight = 0xE00000	// 1110 0000 0000 0000 0000 0000
};

enum class DamageType
{
	Undamaged = 0,
	Light = 1,
	Medium = 2,
	Heavy = 4
};

struct CarDamage
{
	DamageZone Daamge;

	bool IsDamaged(DamageZone zone)
	{
		return ((unsigned int)this->Daamge & (unsigned int)zone) != 0;
	}

	bool IsDamaged(DamageZone zone, DamageType type)
	{
		unsigned int damage = (unsigned int)this->Daamge;
		if (zone == DamageZone::Rear)
		{
			damage = damage >> 3;
		}
		if (zone == DamageZone::FrontLeft)
		{
			damage = damage >> 12;
		}
		if (zone == DamageZone::FrontRight)
		{
			damage = damage >> 15;
		}
		if (zone == DamageZone::RearLeft)
		{
			damage = damage >> 18;
		}
		if (zone == DamageZone::RearRight)
		{
			damage = damage >> 21;
		}

		return (damage & (unsigned int)type) != 0;
	}
};

inline bool operator==(CarDamage& a, CarDamage& b) {
	return a.Daamge == b.Daamge;
}