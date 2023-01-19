#pragma once

#include "Vega/ImGui/ImGuiController.h"

namespace LM
{

	class OGL2ImGuiController : public ImGuiController
	{
	public:
		OGL2ImGuiController(const Ref<Window>& _Window);
		~OGL2ImGuiController();

		virtual void Begin() override;
		virtual void End() override;
	protected:
		Ref<Window> m_Window;
	};

}