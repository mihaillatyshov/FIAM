#pragma once

#include "Vega/Core/Base.h"
#include "Vega/Events/Event.h"

namespace LM
{

	class RendererCommand;

	class Renderer
	{
	public:
		virtual ~Renderer() = default;

		virtual Ref<RendererCommand> CreateRendererCommand() = 0;

		virtual void Clear() = 0;

		virtual void* GetTextureResult() const = 0;
		virtual void OnEvent(const Ref<Event>& _Event) { };

		virtual void Draw() = 0;
	};

}