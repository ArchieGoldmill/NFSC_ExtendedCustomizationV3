#pragma once
#include "Node.h"
#include "Hashes.h"
#include "Math.h"

enum class eLightType : unsigned char
{
	Ambient,
	Directional,
	Omni,
	Spot,
	Exclude,
	SpotHeadlight
};

enum class eLightAttenuation : unsigned char
{
	None,
	Near,
	Far,
	Inverse,
	InverseSquare
};

enum class eLightShape : unsigned char
{
	Circle,
	Rectangle,
	Sphere,
	Aabb
};

enum class eLightState : unsigned char
{
	Off,
	On
};

struct eLight
{
	Hash NameHash;
	eLightType Type;
	eLightAttenuation Attenuation;
	eLightShape Shape;
	eLightState State;
	Hash ExcludeNameHash;
	D3DCOLOR Color;
	D3DXVECTOR3 Position;
	float Size;
	D3DXVECTOR3 Direction;
	float Intensity;
	float FarStart;
	float FarEnd;
	float Falloff;
	unsigned short ScenerySectionNumber;
	char Name[0x22];
};

struct eDynamicLight : bTNode<eDynamicLight>, eLight
{

};

struct eDynamicLightPack : bTNode<eDynamicLightPack>
{
	char Name[32];
	int Enabled;
	bTList<eDynamicLight> DynamicLights;
};