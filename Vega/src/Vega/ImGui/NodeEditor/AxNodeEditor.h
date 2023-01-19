#pragma once

#include "imgui_node_editor.h"

#include "Vega/Core/Base.h"
#include "Vega/ImGui/NodeEditor.h"
#include "Components/ImEdNode.h"

namespace ed = ax::NodeEditor;

namespace LM
{

	class AxNodeEditor : public NodeEditor
	{
	public:
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate() override;
	protected:
		void ImGuiEx_BeginColumn() { ImGui::BeginGroup(); ed::PushStyleVar(ed::StyleVar_PivotAlignment, ImVec2(0.0f, 0.5f)); }
		void ImGuiEx_NextColumn() { ed::PopStyleVar(); ImGui::EndGroup(); ImGui::SameLine(); ImGui::BeginGroup(); ed::PushStyleVar(ed::StyleVar_PivotAlignment, ImVec2(1.0f, 0.5f)); }
		void ImGuiEx_EndColumn() { ed::PopStyleVar(); ImGui::EndGroup(); }
	protected:
		struct LinkInfo
		{
			ed::LinkId Id;
			ed::PinId  InputId;
			ed::PinId  OutputId;
		};
	protected:
		ed::EditorContext*	m_Context = nullptr;    // Editor context, required to trace a editor state.
		bool				m_FirstFrame = true;    // Flag set for first frame only, some action need to be executed once.
		ImVector<LinkInfo>	m_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
		int					m_NextLinkId = 100;     // Counter to help generate link ids. In real application this will probably based on pointer to user data structure.
		std::vector<Ref<ImEdNode>> m_Nodes;
	};

}