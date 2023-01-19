#include "AxNodeEditor.h"

#include "imgui_node_editor_internal.h"
#include "imgui_internal.h"

#include "Vega/Debug/ConsoleLog.h"

namespace LM
{

	void AxNodeEditor::OnAttach()
	{
		ed::Config config;

		config.SettingsFile = "Shader.json";
		m_Context = ed::CreateEditor(&config);
		m_Nodes.emplace_back(CreateRef<ImEdNode>(ImEdNode("TestNode",
			{	{ "IN1" , ImEdPinType::FLOAT } }, 
			{	{ "OUT1", ImEdPinType::INT },
				{ "OUT2", ImEdPinType::FLOAT2 },
				{ "OUT3", ImEdPinType::INT } })));
	}

	void AxNodeEditor::OnDetach()
	{
		ed::DestroyEditor(m_Context);
	}

	void AxNodeEditor::OnUpdate()
	{
		ed::SetCurrentEditor(m_Context);

		//ed::GetGroupMin();
		//ed::GetGroupMax();

		// Start interaction with editor.
		ed::Begin("My Editor", ImVec2(0.0, 0.0f));

		int uniqueId = 1;

		//
		// 1) Commit known data to editor
		//

		// Submit Node A
		ed::NodeId nodeA_Id = uniqueId++;
		ed::PinId  nodeA_InputPinId = uniqueId++;
		ed::PinId  nodeA_OutputPinId = uniqueId++;

		if (m_FirstFrame)
			ed::SetNodePosition(nodeA_Id, ImVec2(10, 10));
		ed::BeginNode(nodeA_Id);
		ImGui::Text("Node A");
		ImGuiEx_BeginColumn();
		ed::BeginPin(nodeA_InputPinId, ed::PinKind::Input); ImGui::Text("-> In"); ed::EndPin();
		ImGuiEx_NextColumn();
		ed::BeginPin(nodeA_OutputPinId, ed::PinKind::Output); ImGui::Text("Out ->"); ed::EndPin();
		ImGuiEx_EndColumn();
		ed::EndNode();

		// Submit Node B
		ed::NodeId nodeB_Id = uniqueId++;
		ed::PinId  nodeB_InputPinId1 = uniqueId++;
		ed::PinId  nodeB_InputPinId2 = uniqueId++;
		ed::PinId  nodeB_InputPinId3 = uniqueId++;
		ed::PinId  nodeB_OutputPinId = uniqueId++;

		if (m_FirstFrame)
			ed::SetNodePosition(nodeB_Id, ImVec2(210, 60));
		ed::BeginNode(nodeB_Id);
		ImGui::Text("Node B");
		ImGuiEx_BeginColumn();
		ed::BeginPin(nodeB_InputPinId1, ed::PinKind::Input); ImGui::Text("-> In1"); ed::EndPin();
		ed::BeginPin(nodeB_InputPinId2, ed::PinKind::Input); ImGui::Text("-> In2"); ed::EndPin();
		ed::BeginPin(nodeB_InputPinId3, ed::PinKind::Input); ImGui::Text("-> Some Big In"); ed::EndPin();
		ImGuiEx_NextColumn();
		ed::BeginPin(nodeB_OutputPinId, ed::PinKind::Output); ImGui::Text("Out ->"); ed::EndPin();
		ImGuiEx_EndColumn();
		ed::EndNode();

		for (auto& Node : m_Nodes)
		{
			Node->Draw();
		}

		// Submit Links
		for (auto& linkInfo : m_Links)
			ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

		//
		// 2) Handle interactions
		//

		// Handle creation action, returns true if editor want to create new object (node or link)
		if (ed::BeginCreate())
		{
			ed::PinId NodeCreate;
			if (ed::QueryNewNode(&NodeCreate))
			{
				LOGW("QueryNewNode");
				if (ed::AcceptNewItem())
				{
					m_Nodes.emplace_back(CreateRef<ImEdNode>(ImEdNode("TestNode",
						{	{ "IN1" , ImEdPinType::FLOAT } },
						{	{ "OUT1", ImEdPinType::INT },
							{ "OUT2", ImEdPinType::FLOAT2 },
							{ "OUT3", ImEdPinType::INT } })));
					m_Nodes.back()->Draw();
				}
			}

			ed::PinId inputPinId, outputPinId;
			if (ed::QueryNewLink(&inputPinId, &outputPinId))
			{
				LOGE("QueryNewLink");
				// QueryNewLink returns true if editor want to create new link between pins.
				//
				// Link can be created only for two valid pins, it is up to you to
				// validate if connection make sense. Editor is happy to make any.
				//
				// Link always goes from input to output. User may choose to drag
				// link from output pin or input pin. This determine which pin ids
				// are valid and which are not:
				//   * input valid, output invalid - user started to drag new ling from input pin
				//   * input invalid, output valid - user started to drag new ling from output pin
				//   * input valid, output valid   - user dragged link over other pin, can be validated

				if (inputPinId && outputPinId) // both are valid, let's accept link
				{
					// ed::AcceptNewItem() return true when user release mouse button.
					if (ed::AcceptNewItem())
					{
						// Since we accepted new link, lets add one to our list of links.
						m_Links.push_back({ ed::LinkId(m_NextLinkId++), inputPinId, outputPinId });

						// Draw new link.
						ed::Link(m_Links.back().Id, m_Links.back().InputId, m_Links.back().OutputId);
					}

					// You may choose to reject connection between these nodes
					// by calling ed::RejectNewItem(). This will allow editor to give
					// visual feedback by changing link thickness and color.
				}
			}
		}
		ed::EndCreate(); // Wraps up object creation action handling.


		// Handle deletion action
		if (ed::BeginDelete())
		{
			// There may be many links marked for deletion, let's loop over them.
			ed::LinkId deletedLinkId;
			while (ed::QueryDeletedLink(&deletedLinkId))
			{
				// If you agree that link can be deleted, accept deletion.
				if (ed::AcceptDeletedItem())
				{
					// Then remove link from your data.
					for (auto& link : m_Links)
					{
						if (link.Id == deletedLinkId)
						{
							m_Links.erase(&link);
							break;
						}
					}
				}

				// You may reject link deletion by calling:
				// ed::RejectDeletedItem();
			}
		}
		ed::EndDelete(); // Wrap up deletion action

		//ax::NodeEditor::Detail::EditorContext* T = ax::NodeEditor::GetCurrentEditor();
		//T->GetContentBounds();
		ImRect R  = ((ax::NodeEditor::Detail::EditorContext*)m_Context)->GetRect();
		ImRect CB = ((ax::NodeEditor::Detail::EditorContext*)m_Context)->GetContentBounds();
		ImRect VR = ((ax::NodeEditor::Detail::EditorContext*)m_Context)->GetViewRect();
		//((ax::NodeEditor::Detail::EditorContext*)m_Context)->GetView();


		// End of interaction with editor.
		ed::End();

		if (m_FirstFrame)
			ed::NavigateToContent(0.0f);
	  
		//if (ImGui::Begin("Ax Params", 0))
		//{
		//    ImGui::DragFloat4("NodePadding", &ed::GetStyle().NodePadding.x);
		//    ImGui::DragFloat2("SourceDirection", &ed::GetStyle().SourceDirection.x);
		//    ImGui::DragFloat2("TargetDirection", &ed::GetStyle().TargetDirection.x);
		//    ImGui::DragFloat2("PivotAlignment", &ed::GetStyle().PivotAlignment.x);
		//    ImGui::DragFloat2("PivotSize", &ed::GetStyle().PivotSize.x);
		//    ImGui::DragFloat2("PivotScale", &ed::GetStyle().PivotScale.x);
		//    ImGui::DragFloat("NodeRounding", &ed::GetStyle().NodeRounding);
		//    ImGui::DragFloat("NodeBorderWidth", &ed::GetStyle().NodeBorderWidth);
		//    ImGui::DragFloat("HoveredNodeBorderWidth", &ed::GetStyle().HoveredNodeBorderWidth);
		//    ImGui::DragFloat("SelectedNodeBorderWidth", &ed::GetStyle().SelectedNodeBorderWidth);
		//    ImGui::DragFloat("PinRounding", &ed::GetStyle().PinRounding);
		//    ImGui::DragFloat("PinBorderWidth", &ed::GetStyle().PinBorderWidth);
		//    ImGui::DragFloat("LinkStrength", &ed::GetStyle().LinkStrength);
		//    ImGui::DragFloat("ScrollDuration", &ed::GetStyle().ScrollDuration);
		//    ImGui::DragFloat("FlowMarkerDistance", &ed::GetStyle().FlowMarkerDistance);
		//    ImGui::DragFloat("FlowSpeed", &ed::GetStyle().FlowSpeed);
		//    ImGui::DragFloat("FlowDuration", &ed::GetStyle().FlowDuration);
		//    ImGui::DragFloat("PinCorners", &ed::GetStyle().PinCorners);
		//    ImGui::DragFloat("PinRadius", &ed::GetStyle().PinRadius);
		//    ImGui::DragFloat("PinArrowSize", &ed::GetStyle().PinArrowSize);
		//    ImGui::DragFloat("PinArrowWidth", &ed::GetStyle().PinArrowWidth);
		//    ImGui::DragFloat("GroupRounding", &ed::GetStyle().GroupRounding);
		//    ImGui::DragFloat("GroupBorderWidth", &ed::GetStyle().GroupBorderWidth);
		//}
		//ImGui::End();

		ed::SetCurrentEditor(nullptr);

		m_FirstFrame = false;

		if (ImGui::Begin("Editor Info"))
		{
			ImGui::Text("CB: %f %f %f %f", CB.Min.x, CB.Min.y, CB.Max.x, CB.Max.y);
			ImGui::Text("R:  %f %f %f %f",  R.Min.x,  R.Min.y,  R.Max.x,  R.Max.y);
			ImGui::Text("VR: %f %f %f %f", VR.Min.x, VR.Min.y, VR.Max.x, VR.Max.y);
		
		}
		ImGui::End();

		// ImGui::ShowMetricsWindow();
	}

}