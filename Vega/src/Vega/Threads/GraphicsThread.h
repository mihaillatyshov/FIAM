#pragma once

#include <thread>

#include "Vega/Core/Window.h"
#include "Vega/ImGui/ImGuiController.h"
#include "Vega/Renderers/Renderer.h"
#include "Vega/Renderers/RendererCommand.h"

namespace LM
{

	class GraphicsThread
	{
	public:
		GraphicsThread(const Ref<EventQueue>& _EventQueue, Ref<Window>& _Window);
		~GraphicsThread();

		inline const Ref<Window>& GetWindow() const { return m_Window; }
		inline const Ref<RendererCommand>& GetRendererCommand() const { return m_RendererCommand; }
		inline const Ref<Renderer>& GetRenderer() const { return m_Renderer; }
		void OnEvent(const Ref<Event>& _Event);
	protected:
		void Init(const Ref<EventQueue>& _EventQueue, Ref<Window>& _Window);
		void Run();
	protected:
		Ref<ImGuiController> m_ImGuiController;
		Ref<Renderer> m_Renderer;
		Ref<RendererCommand> m_RendererCommand;
		Ref<Window> m_Window;

		Scope<EventQueue> m_EventQueue;

		bool m_IsRun = true;
		std::thread m_Thread;
	};

}