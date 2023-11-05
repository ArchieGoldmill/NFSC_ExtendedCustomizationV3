#pragma once
#include <d3d9.h>

#include "Node.h"
#include "Hashes.h"
#include "Func.h"

struct RenderState
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
	RenderState state;
	int type;
	IDirect3DTexture9* pD3DTexture;
	unsigned short punchthruValue;
	unsigned short format;
};

class TextureInfoPlatInterface
{
public:
	TextureInfoPlatInfo* PlatInfo;
};

enum class compression_type : unsigned char
{
	default_ = 0,
	p4 = 4,
	p8 = 8,
	p16 = 16,
	p16_1555 = 17,
	p16_565 = 18,
	p16_3555 = 19,
	rgb24 = 24,
	rgba32 = 32,
	dxt = 33,
	dxt1 = 34,
	dxt3 = 36,
	dxt5 = 38,
	ati2 = 39, // ATI2
	l8 = 40,
	dxt1_air = 41,
	dxt1_aig = 42,
	dxt1_aib = 43,
	ati1 = 44, // ATI1
	p8_16 = 128,
	p8_64 = 129,
	p8_ia8 = 130,
	p4_ia8 = 131,
	p4_rgb24_a8 = 140,
	p8_rgb24_a8 = 141,
	p4_rgb16_a8 = 142,
	p8_rgb16_a8 = 143,
	invalid = 255,
};

enum class tileable_type : unsigned char
{
	clamp = 0x00,
	u_repeat = 0x01,
	v_repeat = 0x02,
	u_mirror = 0x04,
	v_mirror = 0x08,
};

enum class scroll_type : unsigned char
{
	none = 0x00,
	smooth = 0x01,
	snap = 0x02,
	offset_scale = 0x03,
};

enum class alpha_blend_type : unsigned char
{
	src_copy = 0,
	blend = 1,
	additive = 2,
	subtractive = 3,
	overbright = 4,
	dest_blend = 5,
	dest_additive = 6,
	dest_subtractive = 7,
	dest_overbright = 8,
};

enum class alpha_usage_type : unsigned char
{
	none = 0,
	punchthru = 1,
	modulated = 2,
};

enum class bit_flags : unsigned char
{
	unknown = 0x01,
	disable_culling = 0x02,
};

class TextureInfo : public TextureInfoPlatInterface, public bNode< TextureInfo>
{
public:
	inline static TextureInfo* Invalid = (TextureInfo*)-1;

	Hash key;
	Hash class_key;
	int image_placement;
	int palette_placement;
	int image_size;
	int palette_size;
	int base_image_size;
	unsigned short width;
	unsigned short height;
	unsigned char shift_width;
	unsigned char shift_height;
	compression_type image_compression_type;
	unsigned char palette_compression_type;
	unsigned short num_palette_entries;
	unsigned char num_mipmap_levels;
	tileable_type tilable_uv;
	unsigned char bias_level;
	unsigned char rendering_order;
	scroll_type scroll;
	unsigned char used_flag;
	unsigned char apply_alpha_sorting;
	alpha_usage_type usage_type;
	alpha_blend_type blend_type;
	bit_flags flags;
	short scroll_time_step;
	short scroll_speed_s;
	short scroll_speed_t;
	short offset_s;
	short offset_t;
	short scale_s;
	short scale_t;
	short unknown;
	struct pack* pack;
	void* image_data;
	void* palette_data;
	unsigned char name_len;
	char name[35];

	static TextureInfo* Get(Hash hash, bool defaultIfNotFound, bool includeUnloaded)
	{
		FUNC(0x0055CFD0, TextureInfo*, __cdecl, _Get, Hash, bool, bool);
		return _Get(hash, defaultIfNotFound, includeUnloaded);
	}
};