#pragma once

#include "Renderer.h"

namespace LM
{

	class RendererGUI : public Renderer
	{
	public:
		static Ref<RendererGUI> Create();
	};

}