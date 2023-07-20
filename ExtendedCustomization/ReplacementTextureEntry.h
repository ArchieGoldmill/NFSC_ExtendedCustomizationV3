#pragma once
#include "Hashes.h"
#include "TextureInfo.h"
#include "Game.h"

class ReplacementTextureEntry
{
public:
	Hash OldName;
	Hash NewName;
	TextureInfo* Texture;

	void Update(Hash oldName, Hash newName)
	{
		if (this->NewName != newName)
		{
			this->OldName = oldName;
			this->NewName = newName;
			this->Texture = TextureInfo::Invalid;
		}
	}

	void Set(Hash oldName, Hash newName)
	{
		auto textureInfo = GetTextureInfo(newName, false, false);
		if (textureInfo)
		{
			this->OldName = oldName;
			this->NewName = newName;
			this->Texture = textureInfo;
		}
		else
		{
			this->OldName = 0;
			this->NewName = 0;
			this->Texture = 0;
		}
	}
};