#include "Feature.h"
#include "Node.h"
#include "Game.h"
#include "Math.h"
#include "PositionMarker.h"
#include "eModel.h"
#include "CarRenderInfoExtras.h"

void CreateEmitter(CarRenderInfo* carRenderInfo, PositionMarker* marker, bSlist<CarEmitterPosition>* list, int* counter)
{
	(*counter)++;

	auto emiter = (CarEmitterPosition*)bOMalloc(*Game::CarEmitterPositionSlotPool);

	if (emiter)
	{
		emiter->Marker = marker;
		emiter->Position.x = marker->Matrix._41;
		emiter->Position.y = marker->Matrix._42;
		emiter->Position.z = marker->Matrix._43;

		auto tail = list->Tail;
		list->Tail = emiter;
		tail->Next = emiter;
		emiter->Next = (CarEmitterPosition*)list;
	}
	else
	{
		MessageBoxA(NULL, "Unable to allocate CarEmitterPosition!", "Extended Customization", MB_ICONERROR);
		exit(1);
	}

	if (marker->Hash == Hashes::EXHAUST_FX)
	{
		carRenderInfo->Extras->ExhaustFX->CreateSmokeEffect(&marker->Matrix);
	}
}

void CreateEmitter(eModel* model, PositionMarker* fxMarker, bSlist<CarEmitterPosition>* list, int* counter, CarRenderInfo* carRenderInfo)
{
	if (fxMarker->Hash == Hashes::EXHAUST_FX)
	{
		if (strstr(model->Solid->Name, "EXHAUST"))
		{
			if (carRenderInfo->Markers.LeftExhaust)
			{
				auto marker = carRenderInfo->Extras->ExhaustFX->GetAdjustedMarker(fxMarker, carRenderInfo->Markers.LeftExhaust, false);
				CreateEmitter(carRenderInfo, marker, list, counter);
			}

			if (carRenderInfo->Markers.CenterExhaust)
			{
				auto marker = carRenderInfo->Extras->ExhaustFX->GetAdjustedMarker(fxMarker, carRenderInfo->Markers.CenterExhaust, false);
				CreateEmitter(carRenderInfo, marker, list, counter);
			}

			if (carRenderInfo->Markers.RightExhaust)
			{
				auto marker = carRenderInfo->Extras->ExhaustFX->GetAdjustedMarker(fxMarker, carRenderInfo->Markers.RightExhaust, true);
				CreateEmitter(carRenderInfo, marker, list, counter);
			}

			return;
		}
		else if (carRenderInfo->HasExhaustMarker())
		{
			return;
		}
	}

	CreateEmitter(carRenderInfo, fxMarker, list, counter);
}

void CreateBrakelightEmitter(CarRenderInfo* carRenderInfo, PositionMarker* marker)
{
	if (carRenderInfo->Extras->LightTrails)
	{
		if (marker->Hash == Hashes::LEFT_BRAKELIGHT || marker->Hash == Hashes::RIGHT_BRAKELIGHT)
		{
			carRenderInfo->Extras->LightTrails->CreateEffect(&marker->Matrix);
		}
	}
}

int __fastcall GetEmitterPositions(CarRenderInfo* carRenderInfo, int, bSlist<CarEmitterPosition>* list, Hash* hashes, int hashCount)
{
	int count = 0;
	if (carRenderInfo->CarTypeInfo)
	{
		for (int i = 0; i < 0x5A; i++)
		{
			auto model = (eModel*)((unsigned int)(carRenderInfo->PartModel[i][carRenderInfo->MinLodLevel]) & 0xFFFFFFFC);
			if (model)
			{
				PositionMarker* marker = null;
				while (true)
				{
					marker = model->GetNextMarker(marker);
					if (marker)
					{
						CreateBrakelightEmitter(carRenderInfo, marker);

						for (int j = 0; j < hashCount; j++)
						{
							if (marker->Hash == hashes[j])
							{
								CreateEmitter(model, marker, list, &count, carRenderInfo);
							}
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}

	return 0;
}

void __declspec(naked) NosEffectCave()
{
	static constexpr auto cExit = 0x007D5F7C;

	__asm
	{
		mov al, [ecx + 0xB5];
		mov ebp, 0x6B7916BD; // fxcar_nos
		test al, al;
		je NosEffectCave_Exit;
		mov ebp, 0xE3031005; // fxcar_shift

	NosEffectCave_Exit:
		jmp cExit;
	}
}

void __fastcall CarRenderConn_UpdateEffects(CarRenderConn* conn, int, int a2, float a3)
{
	conn->UpdateEffects(a2, a3);

	auto pvehicle = conn->GetPVehicle();
	float speed = pvehicle->GetSpeed();
	auto carRenderInfo = conn->pCarRenderInfo;

	if (abs(speed) < 10)
	{		
		carRenderInfo->Extras->ExhaustFX->UpdateSmoke(conn->Matrix, a3, conn->VelocityVector);
	}

	if (speed > g_Config.LightTrailSpeed)
	{
		carRenderInfo->Extras->LightTrails->Update(conn->Matrix, a3, conn->VelocityVector);
	}
}

void InitExhaust()
{
	if (g_Config.SeparateNosExhaustFX)
	{
		injector::MakeJMP(0x007D5F76, NosEffectCave);
	}

	if (g_Config.FixAutosculptExhaustFX)
	{
		char disableRearBumperCheck[5] = { 0xB8, 0x01, 0x00, 0x00, 0x00 };
		injector::WriteMemoryRaw(0x007CC6BD, disableRearBumperCheck, 5);
		injector::MakeCALL(0x007CC6FC, GetEmitterPositions);
	}

	if (g_Config.ExhaustSmoke)
	{
		injector::MakeCALL(0x007D62AC, CarRenderConn_UpdateEffects);
	}
}