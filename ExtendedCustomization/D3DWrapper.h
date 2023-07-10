#pragma once
#include <D3dx9math.h>

const float M_PI = 3.14159265358979323846f;
#define DTR(a) a * (M_PI / 180) // Degrees to radians
#define RTD(a) a * (180 / M_PI) // Radians to degrees

namespace D3D
{
	class Quaternion
	{
		friend class Matrix;

	private:
		D3DXQUATERNION inst;

	public:
		static Quaternion Slerp(Quaternion q1, Quaternion q2, float t)
		{
			Quaternion result;
			D3DXQuaternionSlerp(&result.inst, &q1.inst, &q2.inst, t);
			return result;
		}

		Quaternion& Normalize()
		{
			D3DXQuaternionNormalize(&this->inst, &this->inst);
			return *this;
		}
	};

	class Matrix
	{
	private:
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
			D3DXMatrixRotationX(&m.inst, DTR(angle));
			return m;
		}

		static Matrix FromRotationY(float angle)
		{
			Matrix m;
			D3DXMatrixRotationY(&m.inst, DTR(angle));
			return m;
		}

		static Matrix FromRotationZ(float angle)
		{
			Matrix m;
			D3DXMatrixRotationZ(&m.inst, DTR(angle));
			return m;
		}

		static Matrix FromScale(D3DXVECTOR3 scale)
		{
			Matrix m;
			D3DXMatrixScaling(&m.inst, scale.x, scale.y, scale.z);
			return m;
		}
	};
}