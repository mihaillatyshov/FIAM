#include "ImGuiController.h"

#include <unordered_map>

#include <imgui.h>
#include <imgui_internal.h>

#include "Vega/Debug/ConsoleLog.h"
#include "Vega/Core/Application.h"
#include "Vega/ImGui/NodeEditor/imgui_node_editor.h"
#include "Vega/ImGui/NodeEditor/AxNodeEditor.h"
#include "Vega/ImGui/Plots/implot.h"

namespace LM
{

	ImGuiController::ImGuiController()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& IO = ImGui::GetIO(); //(void)io;
		//IO.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		//IO.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		//IO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		ImGuiStyle& Style = ImGui::GetStyle();
		if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			Style.WindowRounding = 0.0f;
			Style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		ImFontConfig FontStyle;
		FontStyle.SizePixels = 24;
		auto Font = IO.Fonts->AddFontDefault(&FontStyle);
		IM_ASSERT(Font != NULL);

		m_ImGuiImpl = ImGuiImpl::Create();
		m_ImGuiImpl->OnAttach();
		m_NodeEditor = CreateRef<AxNodeEditor>();
		m_NodeEditor->OnAttach();
		m_MainWindow = CreateRef<ImGuiMainWindow>();

		m_FIAM = FIAM::Get();
	}

	ImGuiController::~ImGuiController()
	{
		m_NodeEditor->OnDetach();
		m_ImGuiImpl->OnDetach();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}

	void ImGuiController::Begin()
	{
		m_ImGuiImpl->Begin();

	}

	void ImGuiController::End()
	{
		ImGuiIO& IO = ImGui::GetIO(); //(void)io;
		IO.DisplaySize = ImVec2((float)Application::Get()->GetWindow()->GetWidth(), (float)Application::Get()->GetWindow()->GetHeight());

		ImGui::Render();

		m_ImGuiImpl->End();
	}

	void ImGuiController::OnUpdate()
	{
		m_MainWindow->Draw();

		ImGui::ShowDemoWindow();

		m_FIAM->OnImGuiDraw();
#if 0
		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_NoDocking;
		ImGuiDockNodeFlags DockFlags = ImGuiDockNodeFlags_NoTabBar | ImGuiDockNodeFlags_NoDocking | ImGuiDockNodeFlags_NoSplit | ImGuiDockNodeFlags_NoDockingSplitMe;
		if (&& ImGui::Begin("Shader Editor", nullptr, WindowFlags))
		{
			static bool FirstLoop = true;
			ImGuiID DockSpaceID = ImGui::GetID("Shader Editor DockSpace"); // The string chosen here is arbitrary (it just gives us something to work with)
			if (FirstLoop)
			{
				ImGui::DockBuilderRemoveNode(DockSpaceID);
				ImGui::DockBuilderAddNode(DockSpaceID);

				std::unordered_map<std::string, ImGuiID> ShaderDockSpace;

				ShaderDockSpace["Attribute"] = ImGui::DockBuilderSplitNode(DockSpaceID, ImGuiDir_Left, 0.15f, nullptr, &DockSpaceID);
				ShaderDockSpace["Varying"] = ImGui::DockBuilderSplitNode(ShaderDockSpace["Attribute"], ImGuiDir_Down, 2.0f / 3.0f, nullptr, &ShaderDockSpace["Attribute"]);
				ShaderDockSpace["Uniforms"] = ImGui::DockBuilderSplitNode(ShaderDockSpace["Varying"], ImGuiDir_Down, 0.5f, nullptr, &ShaderDockSpace["Varying"]);

				ShaderDockSpace["Local Variables"] = ImGui::DockBuilderSplitNode(DockSpaceID, ImGuiDir_Right, 0.15f, nullptr, &DockSpaceID);
				ShaderDockSpace["Procedures"] = ImGui::DockBuilderSplitNode(ShaderDockSpace["Local Variables"], ImGuiDir_Down, 0.75f, nullptr, &ShaderDockSpace["Local Variables"]);
				ShaderDockSpace["Data Types"] = ImGui::DockBuilderSplitNode(ShaderDockSpace["Procedures"], ImGuiDir_Down, 2.0f / 3.0f, nullptr, &ShaderDockSpace["Procedures"]);
				ShaderDockSpace["Component Properties"] = ImGui::DockBuilderSplitNode(ShaderDockSpace["Data Types"], ImGuiDir_Down, 0.5f, nullptr, &ShaderDockSpace["Data Types"]);

				ShaderDockSpace["ShaderNodeEditor"] = DockSpaceID;
				ShaderDockSpace["Debug"] = ImGui::DockBuilderSplitNode(ShaderDockSpace["ShaderNodeEditor"], ImGuiDir_Down, 0.15f, nullptr, &ShaderDockSpace["ShaderNodeEditor"]);

				for (const auto& [Name, Id] : ShaderDockSpace)
				{
					ImGui::DockBuilderDockWindow(Name.c_str(), Id);
					ImGui::DockBuilderGetNode(Id)->LocalFlags |= DockFlags;
				}
				ImGui::DockBuilderGetNode(DockSpaceID)->LocalFlags |= DockFlags;
				ImGui::DockBuilderFinish(DockSpaceID);
				FirstLoop = false;
			}
			ImGui::DockSpace(ImGui::GetID("Shader Editor DockSpace"), ImVec2(0, 0), DockFlags);

			WindowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
			if (ImGui::Begin("Attribute", nullptr, WindowFlags))
			{
				ImGui::Text("Attribute");
				ImGui::Separator();
				if (ImGui::BeginChild("AttributeChild"))
				{
					for (int i = 0; i < 100; i++)
						ImGui::Text("Some text %d", i);
				}
				ImGui::EndChild();
			}
			ImGui::End();

			ImGui::Begin("Varying", nullptr, WindowFlags); ImGui::Text("Varying"); ImGui::Separator(); ImGui::End();
			ImGui::Begin("Uniforms", nullptr, WindowFlags); ImGui::Text("Uniforms"); ImGui::Separator(); ImGui::End();
			ImGui::Begin("ShaderNodeEditor", nullptr, WindowFlags); ImGui::Text("ShaderNodeEditor"); ImGui::Separator(); m_NodeEditor->OnUpdate(); ImGui::End();
			ImGui::Begin("Debug", nullptr, WindowFlags); ImGui::Text("Debug"); ImGui::Separator(); ImGui::End();
			ImGui::Begin("Local Variables", nullptr, WindowFlags); ImGui::Text("Local Variables"); ImGui::Separator(); ImGui::End();
			ImGui::Begin("Procedures", nullptr, WindowFlags); ImGui::Text("Procedures"); ImGui::Separator(); ImGui::End();
			ImGui::Begin("Data Types", nullptr, WindowFlags); ImGui::Text("Data Types"); ImGui::Separator(); ImGui::End();
			ImGui::Begin("Component Properties", nullptr, WindowFlags); ImGui::Text("Component Properties"); ImGui::Separator(); ImGui::End();
		}
		ImGui::End();

		if (ImGui::Begin("Test Window"))
		{
			ImGui::Text("Hello!");
			//ImGui::Image()
		}
		ImGui::End();
#endif
		
	}
}