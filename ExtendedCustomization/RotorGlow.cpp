#include "CarRenderInfo.h"
#include "eModel.h"
#include "CarRenderInfoExtras.h"

void AdjustBrakeScale(CarRenderInfo* carRenderInfo, D3DXMATRIX* transform, int wheelNum)
{
	if (!g_Config.BrakeScale)
	{
		return;
	}

	bool isRear = wheelNum > 1;
	auto wheel = carRenderInfo->pRideInfo->GetPart(isRear ? Slot::REAR_WHEEL : Slot::FRONT_WHEEL);
	auto brake = carRenderInfo->pRideInfo->GetPart(isRear ? Slot::REAR_BRAKE : Slot::FRONT_BRAKE);

	if (isRear && (!wheel || wheel->GetAppliedAttributeBParam(Hashes::DEFAULT, false)))
	{
		wheel = carRenderInfo->pRideInfo->GetPart(Slot::FRONT_WHEEL);
	}

	if ((wheel && !wheel->IsStock() && !wheel->IsAutosculpt()) && (brake && !brake->IsStockByKit()))
	{
		int radius = wheel->GetAppliedAttributeIParam(Hashes::INNER_RADIUS, 0);
		if (radius > 16)
		{
			D3DXVECTOR3 scale, translation;
			D3DXQUATERNION rotation;
			D3DXMatrixDecompose(&scale, &rotation, &translation, transform);

			// Set initial scale
			scale *= 0.85f;

			// Scale to the car's wheel
			scale *= carRenderInfo->WheelData.RadiusScales[wheelNum];

			// Adjust scale for rim size
			scale += scale * (radius - 17) * 0.07f;

			// Clip size for really big rims
			float maxScale = 1.15f;
			if (abs(scale.x) > maxScale)
			{
				scale.x = Sign(scale.x) * maxScale;
				scale.y = Sign(scale.y) * maxScale;
				scale.z = Sign(scale.z) * maxScale;
			}

			D3DXMATRIX result;
			D3DXMatrixScaling(&result, scale.x, scale.y, scale.z);

			D3DXMATRIX rotationMatrix;
			D3DXMatrixRotationQuaternion(&rotationMatrix, &rotation);

			D3DXMatrixMultiply(&result, &result, &rotationMatrix);

			SetVector3(&result, 3, translation);

			*transform = result;
		}
	}
}

void __fastcall RenderCalipers(eView* view, int, CarRenderInfo* carRenderInfo, int reflection, int wheel, eModel* model, D3DXMATRIX* marker, void* light, int flags, int, int)
{
	AdjustBrakeScale(carRenderInfo, marker, wheel);
	model->Render(view, marker, light, flags);
	if (!reflection && carRenderInfo->Extras->RotorGlow)
	{
		carRenderInfo->Extras->RotorGlow->SetMarker(marker);
	}
}

void __fastcall RenderRotors(eView* view, int, CarRenderInfo* carRenderInfo, int reflection, int wheel, eModel* model, D3DXMATRIX* marker, void* light, int flags, int, int)
{
	AdjustBrakeScale(carRenderInfo, marker, wheel);
	model->Render(view, marker, light, flags);
}

void __declspec(naked) RenderCalipersHook()
{
	static constexpr auto cExit = 0x007E0D06;
	__asm
	{
		push[esp + 0x30];
		push[ebp + 0x28];
		push esi;
		call RenderCalipers;

		jmp cExit;
	}
}

void __declspec(naked) RenderRotorsHook()
{
	static constexpr auto cExit = 0x007E0D6C;
	__asm
	{
		push[esp + 0x30];
		push[ebp + 0x28];
		push esi;
		call RenderRotors;

		jmp cExit;
	}
}

void InitRotorGlow()
{
	if (g_Config.RotorGlow || g_Config.BrakeScale)
	{
		injector::MakeJMP(0x007E0D01, RenderCalipersHook, true);
		injector::MakeJMP(0x007E0D67, RenderRotorsHook, true);
	}
}