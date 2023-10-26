#pragma once
#include "Node.h"
#include "Hashes.h"
#include "Func.h"

struct render_state
{
	unsigned int z_write_enabled : 1;
	unsigned int is_backface_culled : 1;
	unsigned int alpha_test_enabled : 1;
	unsigned int alpha_blend_enabled : 1;
	unsigned int alpha_blend_src : 4;
	unsigned int alpha_blend_dest : 4;
	unsigned int texture_address_u : 2;
	unsigned int texture_address_v : 2;
	unsigned int has_texture_animation : 1;
	unsigned int is_additive_blend : 1;
	unsigned int wants_auxiliary_textures : 1;
	unsigned int bias_level : 2;
	unsigned int multi_pass_blend : 1;
	unsigned int colour_write_alpha : 1;
	unsigned int sub_sort_key : 1;
	unsigned int alpha_test_ref : 4;
	unsigned int padding : 4;
};

class TextureInfoPlatInfo : public bNode<TextureInfoPlatInfo>
{
public:
	render_state state;
	int type;
	void* pD3DTexture;
	unsigned short punchthruValue;
	unsigned short format;
};

class TextureInfoPlatInterface
{
public:
	TextureInfoPlatInfo* PlatInfo;
};

class TextureInfo : public TextureInfoPlatInterface, public bNode< TextureInfo>
{
public:
	inline static TextureInfo* Invalid = (TextureInfo*)-1;

	Hash Key;
	Hash ClassKey;

	static TextureInfo* Get(Hash hash, bool defaultIfNotFound, bool includeUnloaded)
	{
		FUNC(0x0055CFD0, TextureInfo*, __cdecl, _Get, Hash, bool, bool);
		return _Get(hash, defaultIfNotFound, includeUnloaded);
	}
};