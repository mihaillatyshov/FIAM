#pragma once

#include <string>

#include "Vega/Core/Base.h"
#include "Vega/Renderers/RendererCommand.h"

namespace LM
{

	class Material
	{
	public:
		virtual ~Material() = default;
		virtual void Add(Ref<Material> _Material, std::string_view _Name, Ref<RendererCommand> _RendererCommand) = 0;
	protected:
	};

}