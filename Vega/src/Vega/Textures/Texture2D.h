#pragma once

#include <Vega/Core/Base.h>
#include "Texture.h"

namespace LM
{

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D();

		virtual void Add(Ref<Texture> _Texture, std::string_view _Name, Ref<RendererCommand> _RendererCommand) override;
		virtual void Remove(std::string_view _Name, Ref<RendererCommand> _RendererCommand) override;
		
		static Ref<Texture2D> Create(std::string_view _FileName, Texture::MASK _Mask = Texture::MASK::NONE);
	};

}