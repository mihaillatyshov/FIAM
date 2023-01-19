#pragma once

#include "Vega/Core/Base.h"
#include "Vega/Core/Window.h"

namespace LM
{

	class ImGuiImpl
	{
	public:
		ImGuiImpl() = default;
		virtual ~ImGuiImpl() = default;

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;

		virtual void Begin() = 0;
		virtual void End() = 0;

		static Ref<ImGuiImpl> Create();
	};

}