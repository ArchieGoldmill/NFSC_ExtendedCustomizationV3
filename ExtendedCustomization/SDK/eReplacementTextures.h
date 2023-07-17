#pragma once
#include "Hashes.h"
#include "TextureInfo.h"

namespace eReplacementTextures
{
	class Entry
	{
	public:
		Hash OldName;
		Hash NewName;
		TextureInfo* Texture;

		void Update( Hash oldName, Hash newName)
		{
			if (this->NewName != newName)
			{
				this->OldName = oldName;
				this->NewName = newName;
				this->Texture = TextureInfo::Invalid;
			}
		}
	};
}