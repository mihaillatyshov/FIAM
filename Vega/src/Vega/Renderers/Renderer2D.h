#pragma once

#include "Renderer.h"

namespace LM
{

	class Renderer2D : public Renderer
	{
	public:
		static Ref<Renderer2D> Create();
	};

}