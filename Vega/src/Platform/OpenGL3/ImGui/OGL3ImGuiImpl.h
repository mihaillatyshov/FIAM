#pragma once

#include "Vega/ImGui/ImGuiImpl.h"

namespace LM
{

	class OGL3ImGuiImpl : public ImGuiImpl
	{
	public:
		OGL3ImGuiImpl();
		virtual ~OGL3ImGuiImpl();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void Begin() override;
		virtual void End() override;
	};

}