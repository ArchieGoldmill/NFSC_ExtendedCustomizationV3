#pragma once
#include <D3dx9math.h>

union Color {
	D3DCOLOR Color;
	BYTE Bytes[4];
};

struct Matrix2
{
public:
	float m[2][2];

	Matrix2 operator*(Matrix2 b)
	{
		Matrix2 c;

		c.m[0][0] = m[0][0] * b.m[0][0] + m[0][1] * b.m[1][0];
		c.m[0][1] = m[0][0] * b.m[0][1] + m[0][1] * b.m[1][1];
		c.m[1][0] = m[1][0] * b.m[0][0] + m[1][1] * b.m[1][0];
		c.m[1][1] = m[1][0] * b.m[0][1] + m[1][1] * b.m[1][1];

		return c;
	}
};

inline D3DXVECTOR4 GetVector(D3DXMATRIX* m, int num)
{
	return D3DXVECTOR4(m->m[num]);
}

inline D3DXVECTOR3 GetVector3(D3DXMATRIX* m, int num)
{
	return D3DXVECTOR3(m->m[num]);
}

inline void SetVector(D3DXMATRIX* m, int num, D3DXVECTOR4 v)
{
	memcpy(m->m[num], &v, sizeof(D3DXVECTOR4));
}

inline void SetVector3(D3DXMATRIX* m, int num, D3DXVECTOR3 v)
{
	memcpy(m->m[num], &v, sizeof(D3DXVECTOR3));
}

inline void MoveTowards(float& a, float b, float step)
{
	if (a < b)
	{
		a += step;
		if (a > b)
		{
			a = b;
		}
	}

	if (a > b)
	{
		a -= step;
		if (a < b)
		{
			a = b;
		}
	}
}

inline int ToInt(float a)
{
	return (int)(a + 0.5f);
}