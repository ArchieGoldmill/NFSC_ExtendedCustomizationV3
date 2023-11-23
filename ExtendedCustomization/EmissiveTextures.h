#pragma once
#include "RenderingModel.h"

DynamicLightingContext PrelitContext, InteriorContext;
bool PrelitInit = false;

void InitLightingContext(DynamicLightingContext* context, float light)
{
	memset(context, 0, sizeof(DynamicLightingContext));
	for (int i = 0; i < 10; i++)
	{
		context->Harmonics[i] = { light, light, light, light };
	}
}

void InitPrelit()
{
	if (!PrelitInit)
	{
		InitLightingContext(&PrelitContext, 1.0f);
		InitLightingContext(&InteriorContext, 0.2f);
		PrelitInit = true;
	}
}

int GetLastChar(char* str, char c)
{
	int i = 0;
	int res = -1;
	while (*str)
	{
		if (*str == c)
		{
			res = i;
		}

		i++;
		str++;
	}

	return res;
}

void CreateGenericRenderingModel(void* meshEntry, eSolid* solid, int flags, void* effect, TextureInfo** textures, D3DXMATRIX* trs, DynamicLightingContext* context, eLightMaterial* material, void* blend_trs, void* pca)
{
	if (material->NameHash == Hashes::INTERIOR && Game::InRace())
	{
		context = &InteriorContext;
	}

	RenderingModel::Create(meshEntry, solid, flags, effect, textures, trs, context, material, blend_trs, pca);
}

void CreateEmissiveRenderingModel(void* meshEntry, eSolid* solid, int flags, void* effect, TextureInfo** textures, D3DXMATRIX* trs, DynamicLightingContext* context, eLightMaterial* material, void* blend_trs, void* pca)
{
	material = eLightMaterial::Get(StringHash("EMISSIVE"), 0);
	context = &PrelitContext;
	RenderingModel::Create(meshEntry, solid, flags, effect, textures, trs, context, material, blend_trs, pca);
}

void __cdecl CreateRenderingModel(void* meshEntry, eSolid* solid, int flags, void* effect, TextureInfo** textures, D3DXMATRIX* trs, DynamicLightingContext* context, eLightMaterial* material, void* blend_trs, void* pca)
{
	InitPrelit();

	auto original = textures[0];
	auto classKey = original->class_key;
	if (classKey == StringHash("Emissive On"))
	{
		char name[64];
		strcpy(name, original->name);

		int last = GetLastChar(name, '_');
		strcpy(name + last + 1, "OFF");

		auto offTexture = TextureInfo::Get(StringHash(name), false, false);
		if (offTexture)
		{
			textures[0] = offTexture;
			CreateGenericRenderingModel(meshEntry, solid, flags, effect, textures, trs, context, material, blend_trs, pca);

			textures[0] = original;
			CreateEmissiveRenderingModel(meshEntry, solid, flags, effect, textures, trs, context, material, blend_trs, pca);
			return;
		}
	}

	if (classKey == StringHash("Emissive"))
	{
		CreateEmissiveRenderingModel(meshEntry, solid, flags, effect, textures, trs, context, material, blend_trs, pca);
		return;
	}

	CreateGenericRenderingModel(meshEntry, solid, flags, effect, textures, trs, context, material, blend_trs, pca);
	auto model = RenderingModel::GetLast();

	auto key = model->diffuse_texture_info->key;
	key = StringHash1("_E", key);
	auto emissive = TextureInfo::Get(key, false, false);
	if (emissive)
	{
		textures[0] = emissive;
		CreateEmissiveRenderingModel(meshEntry, solid, flags, effect, textures, trs, context, material, blend_trs, pca);
	}
}

void InitEmissiveTextures()
{
	injector::MakeCALL(0x007295EE, CreateRenderingModel);
}