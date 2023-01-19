#include "OGL4ImGuiImpl.h"

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Vega/Core/Application.h"

namespace LM
{

	OGL4ImGuiImpl::OGL4ImGuiImpl()
	{
	}

	OGL4ImGuiImpl::~OGL4ImGuiImpl()
	{

	}

	void OGL4ImGuiImpl::OnAttach()
	{
		ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Application::Get()->GetWindow()->GetNativeWindow()), true);
		ImGui_ImplOpenGL3_Init("#version 450 core");
	}

	void OGL4ImGuiImpl::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}

	void OGL4ImGuiImpl::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void OGL4ImGuiImpl::End()
	{
		ImGuiIO& IO = ImGui::GetIO(); //(void)io;
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* BackupCurrentContext = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(BackupCurrentContext);
		}
	}

}