#include "GraphicsThread.h"

#include <future>

#include "Vega/Renderers/Renderer3D/Renderer3D.h"
#include "Vega/Core/Application.h"

#include "Vega/Events/EventDispatcher.h"
#include "Vega/Events/WindowEvent.h"

namespace LM
{

	GraphicsThread::GraphicsThread(const Ref<EventQueue>& _EventQueue, Ref<Window>& _Window)
	{
		std::packaged_task<void(void)> InitTask([&]() { Init(_EventQueue, _Window); });
		auto InitEnd = InitTask.get_future();
		m_Thread = std::thread([&]() { InitTask(); Run(); });
		InitEnd.get();
	}

	GraphicsThread::~GraphicsThread()
	{
		if (m_Thread.joinable())
			m_Thread.join();
	}

	void GraphicsThread::Init(const Ref<EventQueue>& _EventQueue, Ref<Window>& _Window)
	{
		m_EventQueue = CreateScope<EventQueue>();

		m_Window = Window::Create();
		m_Window->SetEventCallback(_EventQueue);
		LOGI("Window Created");
		_Window = m_Window;
		LOGI("Window attached to Application");

		m_Renderer = Renderer3D::Create();
		LOGI("Renderer Created");
		m_RendererCommand = m_Renderer->CreateRendererCommand();

		m_ImGuiController = CreateRef<ImGuiController>();
		LOGI("GraphicsThread Id: ", std::this_thread::get_id());
	}

	void GraphicsThread::Run()
	{
		while (m_IsRun)
		{
			while (Ref<Event> E = m_EventQueue->TryPop())
			{
				m_Renderer->OnEvent(E);
				EventDispatcher EDisp(E);
				EDisp.Dispatch<WindowCloseEvent>([&](Ref<WindowCloseEvent>) {
					LOGI("EXIT");
					m_IsRun = false;
					return false;
					});
			}
			m_RendererCommand->UpdateRendererData(m_Renderer);
			m_Renderer->Draw();
			m_ImGuiController->Begin();
			m_ImGuiController->OnUpdate();
			m_ImGuiController->End();
			m_Window->OnUpdate();
		}
	}

	void GraphicsThread::OnEvent(const Ref<Event>& _Event)
	{
		m_EventQueue->Add(_Event);
	}

}