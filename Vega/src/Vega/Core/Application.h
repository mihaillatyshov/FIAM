#pragma once

#include "Window.h"
#include "Vega/Threads/GraphicsThread.h"
#include "Vega/Layers/LayerStack.h"

namespace LM
{

	class Application
	{
	public:
		Application();

		void Run();

		// Get Renderer Command
		// @return Get Renderer Command from graphics thread
		inline Ref<RendererCommand> GetRendererCommand() { return m_Graphics->GetRendererCommand(); }
		inline Ref<Renderer> GetRenderer() { return m_Graphics->GetRenderer(); }

		inline Ref<Window> GetWindow() { return m_Window; }
		inline void SetWindow(const Ref<Window>& _Window) { m_Window = _Window; }

		inline void AddEvent(const Ref<Event>& _Event) { m_EventQueue->Add(_Event); }

		// Get Application instance
		static Application* Get() { return s_Instance; }
	protected:
		void Init();
	protected:
		Ref<GraphicsThread> m_Graphics;
		Ref<Window> m_Window;
		Ref<LayerStack> m_LayerStack;

		Ref<EventQueue> m_EventQueue;

		static inline Application* s_Instance;
	};

	Application* CreateApplication();
}