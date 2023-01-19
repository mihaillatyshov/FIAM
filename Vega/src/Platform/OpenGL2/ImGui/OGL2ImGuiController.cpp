#include "OGL2ImGuiController.h"

#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <imgui.h>
#include <glm.hpp>

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl2.h"
#include "Vega/Debug/ConsoleLog.h"

namespace LM
{

	OGL2ImGuiController::OGL2ImGuiController(const Ref<Window>& _Window)
		: m_Window(_Window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
		auto Font = io.Fonts->AddFontDefault();
		IM_ASSERT(Font != NULL);

		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()), true);
		ImGui_ImplOpenGL2_Init();
	}

	OGL2ImGuiController::~OGL2ImGuiController()
	{
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void OGL2ImGuiController::Begin()
	{
		//glUseProgram(0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//glActiveTexture(GL_TEXTURE0);

		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (ImGui::Begin("Test"))
		{
			static float TestFloat = 10.0f;
			ImGui::DragFloat("TestFloat", &TestFloat);
		}
		ImGui::End();
	}
	
	void OGL2ImGuiController::End()
	{
		ImGuiIO& io = ImGui::GetIO(); //(void)io;
		io.DisplaySize = ImVec2((float)m_Window->GetWidth(), (float)m_Window->GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}