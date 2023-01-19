#pragma once

#include <cstdint>
#include <string>

#include "Vega/Core/Base.h"
#include "Vega/Events/EventQueue.h"

namespace LM
{

	enum class WindowMSAA : int
	{
		None = 0,
		X2 = 2,
		X4 = 4,
		X8 = 8
	};

	struct WindowProps
	{
		std::string Title = "Vega";
		uint32_t Width = 1280u;
		uint32_t Height = 720u;
		WindowMSAA MSAA = WindowMSAA::None;
	};

	class Window
	{
	public:
		virtual ~Window() = default;

		virtual uint32_t GetWidth()  const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual WindowMSAA GetMSAA() const = 0;

		virtual void OnUpdate() = 0;

		virtual void SetEventCallback(const Ref<EventQueue> _EventQueue) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Ref<Window> Create(const WindowProps& _Props = WindowProps());
	};

}