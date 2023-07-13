#pragma once
#include <D3dx9math.h>

namespace D3D
{
	class Quaternion
	{
		friend class Matrix;

	private:
		D3DXQUATERNION inst;

	public:

		Quaternion& Normalize()
		{
			D3DXQuaternionNormalize(&this->inst, &this->inst);
			return *this;
		}

		static Quaternion Slerp(Quaternion q1, Quaternion q2, float t)
		{
			Quaternion result;
			D3DXQuaternionSlerp(&result.inst, &q1.inst, &q2.inst, t);
			return result;
		}
	};

	class Matrix
	{
	public:
		D3DXMATRIX inst;

	public:
		Quaternion ToQuaternion()
		{
			Quaternion quaternion;
			D3DXQuaternionRotationMatrix(&quaternion.inst, &this->inst);
			return quaternion;
		}

		D3DXVECTOR3 GetW()
		{
			D3DXVECTOR3 v;
			v.x = this->inst.m[3][0];
			v.y = this->inst.m[3][1];
			v.z = this->inst.m[3][2];

			return v;
		}

		void SetW(D3DXVECTOR3 t)
		{
			this->inst.m[3][0] = t.x;
			this->inst.m[3][1] = t.y;
			this->inst.m[3][2] = t.z;
		}

		void Transform(D3DXVECTOR4& v)
		{
			if (v.w == 0)
			{
				v.w = 1;
			}

			D3DXVec3TransformCoord((D3DXVECTOR3*)&v, (D3DXVECTOR3*)&v, &this->inst);
		}

		void Inverse()
		{
			D3DXMatrixInverse(&this->inst, NULL, &this->inst);
		}

		static Matrix Transformation(Quaternion q, D3DXVECTOR3 origin)
		{
			Matrix result;
			D3DXVECTOR3 pos{ 0,0,0 };
			D3DXMatrixTransformation(&result.inst, NULL, NULL, NULL, &origin, &q.inst, &pos);
			return result;
		}

		static void Multiply(Matrix* result, Matrix* m1, Matrix* m2)
		{
			D3DXMatrixMultiply(&result->inst, &m1->inst, &m2->inst);
		}

		static Matrix FromRotationX(float angle)
		{
			Matrix m;
			D3DXMatrixRotationX(&m.inst, D3DXToRadian(angle));
			return m;
		}

		static Matrix FromRotationY(float angle)
		{
			Matrix m;
			D3DXMatrixRotationY(&m.inst, D3DXToRadian(angle));
			return m;
		}

		static Matrix FromRotationZ(float angle)
		{
			Matrix m;
			D3DXMatrixRotationZ(&m.inst, D3DXToRadian(angle));
			return m;
		}

		static Matrix FromScale(D3DXVECTOR3 scale)
		{
			Matrix m;
			D3DXMatrixScaling(&m.inst, scale.x, scale.y, scale.z);
			return m;
		}

		static Matrix Identity()
		{
			Matrix m;
			D3DXMatrixScaling(&m.inst, 1, 1, 1);
			return m;
		}

		static Matrix FromLookAt(D3DXVECTOR3 a, D3DXVECTOR3 b)
		{
			Matrix m;
			D3DXVECTOR3 up{ 0, 0, 1 };
			D3DXMatrixLookAtLH(&m.inst, &a, &b, &up);
			D3DXMatrixInverse(&m.inst, NULL, &m.inst);
			return m;
		}

		static Matrix FromRotationAxis(D3DXVECTOR3 axis, float angle)
		{
			Matrix m;
			D3DXMatrixRotationAxis(&m.inst, &axis, angle);
			return m;
		}
	};
}

struct bVector2
{
	float x;
	float y;
};

struct bVector3
{
	float x;
	float y;
	float z;
	float pad;
};

struct bVector4
{
	float x;
	float y;
	float z;
	float w;
};

struct bMatrix4
{
	bVector4 v0;
	bVector4 v1;
	bVector4 v2;
	bVector4 v3;
};
