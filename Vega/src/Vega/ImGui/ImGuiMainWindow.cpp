#include "ImGuiMainWindow.h"

#include <unordered_map>

#include <imgui.h>
#include <imgui_internal.h>

#include "Vega/Core/Application.h"
#include "Vega/Events/WindowEvent.h"

namespace LM
{

	void ImGuiMainWindow::Draw()
	{
		if (ImGui::BeginMainMenuBar())
		{
			ImGui::MenuItem("Test");
		}
		ImGui::EndMainMenuBar();
		ImGuiID MainDockSpace = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		Init(MainDockSpace);
		ImGui::Begin("Scene"); ImGui::End();
		ImGui::Begin("Props"); ImGui::End();
		ImGui::Begin("Assets"); ImGui::End();
		ImGui::Begin("Components"); ImGui::End();
		ImGui::Begin("StartPanel"); ImGui::End();
		if (ImGui::Begin("Viewport"))
		{
			ImVec2 Size = ImGui::GetContentRegionAvail();
			Application::Get()->AddEvent(CreateRef<WindowResizeEvent>(Size.x, Size.y));
			ImGui::Image(Application::Get()->GetRenderer()->GetTextureResult(), { Size.x, Size.y }, { 0, 1 }, { 1, 0 });
		}
		ImGui::End();
	}

	void ImGuiMainWindow::Init(uint32_t DockSpaceId)
	{
		if (ImGui::DockBuilderGetNode(DockSpaceId) == NULL)
		{
			ImGui::DockBuilderRemoveNode(DockSpaceId);
			ImGui::DockBuilderAddNode(DockSpaceId);

			std::unordered_map<std::string, ImGuiID> DockSpaceMap;

			ImGuiID ViewportId;
			DockSpaceMap["Scene"] = ImGui::DockBuilderSplitNode(DockSpaceId, ImGuiDir_Right, 0.2f, NULL, &ViewportId);
			DockSpaceMap["Props"] = ImGui::DockBuilderSplitNode(DockSpaceMap["Scene"], ImGuiDir_Down, 0.6f, NULL, &DockSpaceMap["Scene"]);
			DockSpaceMap["Assets"] = ImGui::DockBuilderSplitNode(ViewportId, ImGuiDir_Down, 0.2f, NULL, &ViewportId);
			DockSpaceMap["Components"] = ImGui::DockBuilderSplitNode(ViewportId, ImGuiDir_Left, 0.2f, NULL, &ViewportId);
			DockSpaceMap["StartPanel"] = ImGui::DockBuilderSplitNode(ViewportId, ImGuiDir_Up, 0.2f, NULL, &ViewportId);
			DockSpaceMap["Viewport"] = ViewportId;

			for (const auto& [Name, Id] : DockSpaceMap)
			{
				ImGui::DockBuilderDockWindow(Name.c_str(), Id);
			}
			ImGui::DockBuilderFinish(DockSpaceId);
		}
	}
}