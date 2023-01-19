#pragma once

#include <string_view>

#include "Vega/Events/Event.h"

namespace LM
{

	class Layer
	{
	public:
		Layer(std::string_view _Name = "") : m_Name(_Name) { }
		virtual ~Layer() = default;

		virtual void OnAttach() { }
		virtual void OnDetach() { }
		virtual void OnUpdate() { }
		virtual void OnEvent(Ref<Event> _Event) { }
	protected:
		std::string m_Name;
	};

}