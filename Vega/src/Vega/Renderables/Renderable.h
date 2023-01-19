#pragma once

#include "Vega/Core/Base.h"
#include "Vega/Game/Transformation.h"
#include "Vega/Renderers/RendererCommand.h"

namespace LM
{

	class Renderable
	{
	public:
		virtual void Add(Ref<Renderable> _Renderable, Ref<Transformation> _Transformation, Ref<RendererCommand> _RendererCommand) = 0;
	protected:

	};

}