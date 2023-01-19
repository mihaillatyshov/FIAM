#pragma once

#include "Vega/ImGui/ImGuiImpl.h"

namespace LM
{

	class OGL4ImGuiImpl : public ImGuiImpl
	{
	public:
		OGL4ImGuiImpl();
		virtual ~OGL4ImGuiImpl();

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void Begin() override;
		virtual void End() override;
	};

}