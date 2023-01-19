#include "GLFWWindow.h"

#include <GL/glew.h>

#include "Vega/Debug/ConsoleLog.h"
#include "Vega/Events/WindowEvent.h"
#include "Vega/Events/KeyEvent.h"
#include "Vega/Events/MouseEvent.h"

namespace LM
{

	GLFWWindow::GLFWWindow(const WindowProps& _Props)
		: m_Data(_Props)
	{
		Init();
	}

	GLFWWindow::~GLFWWindow()
	{
		glfwDestroyWindow(m_Window);
		//glfwTerminate();
		
		//delete m_Window;
	}

	void GLFWWindow::OnUpdate()
	{
		glfwMakeContextCurrent(m_Window);
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	bool GLFWWindow::Init()
	{
		if (!glfwInit()) {
			LOGE("Failed to initialize GLFW!");
			return false;
		}

		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		if (m_Data.MSAA != WindowMSAA::None)
			glfwWindowHint(GLFW_SAMPLES, (int)m_Data.MSAA);

		m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), NULL, NULL);

		if (!m_Window) {
			LOGE("Failed to create window!");
			return false;
		}

		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);

		SetCallbacks();
		//glfwSetErrorCallback(glfw_error_callback);
		//glfwSetFramebufferSizeCallback(m_Window, window_resize);
		//glfwSetWindowSizeCallback(m_Window, window_resize);
		//glfwSetKeyCallback(m_Window, key_callback);
		//glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
		//glfwSetCursorPosCallback(m_Window, cursor_position_callback);
		//glfwSetScrollCallback(m_Window, scroll_callback);
		//glfwSetCharCallback(m_Window, character_input);
		//glfwSetDropCallback(m_Window, droping_paths);

		//glfwSetErrorCallback
		//glfwSetFramebufferSizeCallback
		//glfwSetDropCallback

		glfwSwapInterval(0);

		if (glewInit() != GLEW_OK) {
			LOGE("Could not initialise GLEW!");
			return false;
		}

		glEnable(GL_BLEND);
		//glEnable(GL_ALPHA_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		LOGI("OpenGL version: ", glGetString(GL_VERSION));
		LOGI("OpenGL vendor: ", glGetString(GL_VENDOR));
		LOGI("OpenGL renderer: ", glGetString(GL_RENDERER));
		//LOGI("OpenGL extensions: ", glGetString(GL_EXTENSIONS));
		LOGI("OpenGL shading language version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

		GLint texture_units;
		glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texture_units);
		LOGI("Number of textures can be acessed by fragment shader: ", texture_units);
		glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &texture_units);
		LOGI("Max texture units: ", texture_units);

		return true;
	}

	void GLFWWindow::SetCallbacks()
	{
		//glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* _Window, int _Width, int _Height) 
		//{
		//	WindowData* Data = (WindowData*)glfwGetWindowUserPointer(_Window);
		//	Data->Width = _Width;
		//	Data->Height = _Height;
		//	Data->EQueue->Add(CreateRef<WindowResizeEvent>(_Width, _Height));
		//});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* _Window)
		{
			WindowData* Data = (WindowData*)glfwGetWindowUserPointer(_Window);
			Data->EQueue->Add(CreateRef<WindowCloseEvent>());
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* _Window, int _Key, int _ScanCode, int _Action, int _Mods)
		{
			WindowData* Data = (WindowData*)glfwGetWindowUserPointer(_Window);
			switch (_Action)
			{
				case GLFW_PRESS:	Data->EQueue->Add(CreateRef<KeyPressedEvent>(_Key, 0)); break;
				case GLFW_RELEASE:	Data->EQueue->Add(CreateRef<KeyReleasedEvent>(_Key));	break;
				case GLFW_REPEAT:	Data->EQueue->Add(CreateRef<KeyPressedEvent>(_Key, 1)); break;
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* _Window, uint32_t _KeyCode)
		{
			WindowData* Data = (WindowData*)glfwGetWindowUserPointer(_Window);
			Data->EQueue->Add(CreateRef<KeyTypedEvent>(_KeyCode));
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* _Window, int _Button, int _Action, int _Mods)
		{
			WindowData* Data = (WindowData*)glfwGetWindowUserPointer(_Window);
			switch (_Action)
			{
				case GLFW_PRESS:	Data->EQueue->Add(CreateRef<MouseButtonPressedEvent>(_Button));		break;
				case GLFW_RELEASE:	Data->EQueue->Add(CreateRef<MouseButtonReleasedEvent>(_Button));	break;
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* _Window, double _OffsetX, double _OffsetY)
		{
			WindowData* Data = (WindowData*)glfwGetWindowUserPointer(_Window);
			Data->EQueue->Add(CreateRef<MouseScrolledEvent>((float)_OffsetX, (float)_OffsetY));
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* _Window, double _PosX, double _PosY)
		{
			WindowData* Data = (WindowData*)glfwGetWindowUserPointer(_Window);
			Data->EQueue->Add(CreateRef<MouseMovedEvent>((float)_PosX, (float)_PosY));
		});
	}

}