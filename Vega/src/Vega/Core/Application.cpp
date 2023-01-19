#include "Application.h"

#include <iostream>
#include <chrono>

#include "Vega/Renderers/RendererAPI.h"
#include "Vega/Debug/ConsoleLog.h"
#include "Vega/Events/EventDispatcher.h"
#include "Vega/Events/WindowEvent.h"
#include "Vega/Events/KeyEvent.h"

using namespace std::chrono_literals;

namespace LM
{

	Application::Application()
	{
		s_Instance = this;
		LOG_INIT();
		FIAM::Init();
		Init();
		LOGI("Application created!!!");
		m_LayerStack = CreateRef<LayerStack>();
	}

	void Application::Init()
	{
		RendererAPI::Set(RendererAPI::API::OpenGL4);
		m_EventQueue = CreateRef<EventQueue>();
		LOGI("EventQueue Created");
		m_Graphics = CreateRef<GraphicsThread>(m_EventQueue, m_Window);
		LOGI("GraphicsThread Created");
		//m_Window = m_Graphics->GetWindow();
	}

	void Application::Run()
	{
		srand(time(NULL));

		m_LayerStack->OnAttach();

		while (true)
		{
			//std::this_thread::sleep_for(16ms);						// if we need ticks in game
			bool NeedClose = false;
			while (Ref<Event> E = m_EventQueue->TryPop())
			{
				EventDispatcher EDisp(E);
				EDisp.Dispatch<WindowCloseEvent>([&](Ref<WindowCloseEvent>) {
					LOGI("EXIT");
					NeedClose = true;
					return false;
					});
				m_Graphics->OnEvent(E);
				m_LayerStack->OnEvent(E);
				//LOGI(E->ToString());
			}
			if (NeedClose)
			{
				//m_Graphics->Close();
				break;
			}
			m_LayerStack->OnUpdate();
			// {
			//		Physics.OnUpdate();
			//		AI.OnUpdate();
			//		Animations.UnUpdate();
			//		Audio.OnUpdate();
			// }
			// Send update stack to graphics there or in place there we need???

		}
	}

}