#include "Feature.h"
#include "Node.h"
#include "Game.h"
#include "Math.h"
#include "PositionMarker.h"
#include "eModel.h"
#include "CarRenderInfoExtras.h"

struct CarEmitterPosition : bSNode< CarEmitterPosition>
{
	D3DVECTOR Position;
	PositionMarker* Marker;
};

void CreateEmitter(PositionMarker* marker, bSlist<CarEmitterPosition>* list, int* counter)
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
		MessageBoxA(NULL, "Unable to allocate 'CarEmitterPosition'!", "NFSC - Style Point", MB_ICONERROR);
		exit(1);
	}
}

void __stdcall GetEmitterPositions(eModel* model, PositionMarker* fxMarker, bSlist<CarEmitterPosition>* list, int* counter, CarRenderInfo* carRenderInfo)
{
	if (fxMarker->Hash == Hashes::EXHAUST_FX)
	{
		if (carRenderInfo->HasExhaustMerker())
		{
			if (strstr(model->Solid->Name, "EXHAUST_STYLE"))
			{
				if (carRenderInfo->Markers.LeftExhaust)
				{
					auto marker = carRenderInfo->Extras->ExhaustFX->GetAdjustedMarker(fxMarker, carRenderInfo->Markers.LeftExhaust, false);
					CreateEmitter(marker, list, counter);
				}

				if (carRenderInfo->Markers.CenterExhaust)
				{
					auto marker = carRenderInfo->Extras->ExhaustFX->GetAdjustedMarker(fxMarker, carRenderInfo->Markers.CenterExhaust, false);
					CreateEmitter(marker, list, counter);
				}

				if (carRenderInfo->Markers.RightExhaust)
				{
					auto marker = carRenderInfo->Extras->ExhaustFX->GetAdjustedMarker(fxMarker, carRenderInfo->Markers.RightExhaust, true);
					CreateEmitter(marker, list, counter);
				}
			}

			return;
		}
	}

	CreateEmitter(fxMarker, list, counter);
}

void __declspec(naked) GetEmitterPositionsCave()
{
	static constexpr auto cExit = 0x007BEC35;
	__asm
	{
		pushad;

		push[esp + 0x38];
		mov eax, esp;
		add eax, 0x34;
		push eax; //counter ptr
		push ebx;
		push esi; //mount point
		push ebp; //model
		call GetEmitterPositions;

		popad;
		jmp cExit;
	}
}

void __declspec(naked) NosEffectCave()
{
	static constexpr auto cExit = 0x007D5F7C;

	__asm
	{
		mov al, [ecx + 0xB5];
		mov ebp, 0xE3031005; // fxcar_shift
		test al, al;
		je NosEffectCave_Exit;
		mov ebp, 0x6B7916BD; // fxcar_nos

	NosEffectCave_Exit:
		jmp cExit;
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
		injector::WriteMemoryRaw(0x007CC6BD, disableRearBumperCheck, 5, true);
		injector::MakeJMP(0x007BEBFB, GetEmitterPositionsCave, true);
	}
}