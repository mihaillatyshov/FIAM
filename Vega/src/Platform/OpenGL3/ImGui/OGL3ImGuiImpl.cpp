#include "OGL3ImGuiImpl.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Vega/Core/Application.h"

namespace LM
{

	OGL3ImGuiImpl::OGL3ImGuiImpl()
	{
	}

	OGL3ImGuiImpl::~OGL3ImGuiImpl()
	{

	}

	void OGL3ImGuiImpl::OnAttach()
	{
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow()), true);
		ImGui_ImplOpenGL3_Init("#version 330 core");
	}

	void OGL3ImGuiImpl::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}

	void OGL3ImGuiImpl::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void OGL3ImGuiImpl::End()
	{
		ImGuiIO& io = ImGui::GetIO(); //(void)io;
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

}