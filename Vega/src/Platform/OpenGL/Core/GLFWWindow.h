#pragma once

#include <GLFW/glfw3.h>

#include "Vega/Core/Window.h"

namespace LM
{

	class GLFWWindow : public Window
	{
	public:
		GLFWWindow(const WindowProps& _Props);
		virtual ~GLFWWindow();

		virtual uint32_t GetWidth()  const override { return m_Data.Width; }
		virtual uint32_t GetHeight() const override { return m_Data.Height; }
		virtual WindowMSAA GetMSAA() const override { return m_Data.MSAA; }

		virtual void OnUpdate() override;

		virtual void SetEventCallback(const Ref<EventQueue> _EventQueue) override { m_Data.EQueue = _EventQueue; }

		virtual void* GetNativeWindow() const override { return m_Window; }
	protected:
		bool Init();
		void SetCallbacks();
	protected:
		struct WindowData : WindowProps
		{
			WindowData(const WindowProps& _Props)
				: WindowProps(_Props) { }
			Ref<EventQueue> EQueue;
		};

		GLFWwindow* m_Window;

		WindowData m_Data;
	};

}