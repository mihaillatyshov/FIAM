#pragma once

#include <unordered_map>

#include "Texture.h"

namespace LM
{

	class TextureManager
	{
	public:
		void Add(Ref<Texture> _Texture, std::string_view _Name) { m_Textures[_Name.data()] = _Texture; }
		void Remove(std::string_view _Name) { m_Textures.erase(_Name.data()); }
	protected:
		std::unordered_map<std::string, Ref<Texture>> m_Textures;
	};

}