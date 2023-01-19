#include "ImEdNode.h"

namespace LM
{

	ImEdNode::ImEdNode(std::string_view _Name, const std::initializer_list<ImEdPinProps>& _InPins, const std::initializer_list<ImEdPinProps>& _OutPins)
		: m_Name(_Name)
	{
		for (const auto& Prop : _InPins)
		{
			m_InPins.emplace_back(Prop, ed::PinKind::Input);
		}
		for (const auto& Prop : _OutPins)
		{
			m_OutPins.emplace_back(Prop, ed::PinKind::Output);
		}
	}

	void ImEdNode::Draw()
	{
		if (m_FirstFrame)
		{
			ed::SetNodePosition(GetID(), ImVec2(150, 60));
			m_FirstFrame = false;
		}
		
		ed::BeginNode(GetID());
		ImGui::Text(m_Name.data());
		ImEdBeginColumn();
		for (int i = 0; i < m_InPins.size(); i++)
		{
			ed::BeginPin(m_InPins[i].GetID(), ed::PinKind::Input);
			ImGui::Text("-> %s", m_InPins[i].GetName().data());
			ed::EndPin();
		}
		ImEdNextColumn();
		for (int i = 0; i < m_OutPins.size(); i++)
		{
			ed::BeginPin(m_OutPins[i].GetID(), ed::PinKind::Output);
			ImGui::Text("%s ->", m_OutPins[i].GetName().data());
			ed::EndPin();
		}
		ImEdEndColumn();
		ed::EndNode();
	}

	void ImEdNode::ImEdBeginColumn() 
	{ 
		ImGui::BeginGroup(); 
		ed::PushStyleVar(ed::StyleVar_PivotAlignment, ImVec2(0.0f, 0.5f)); 
	}

	void ImEdNode::ImEdNextColumn() 
	{ 
		ed::PopStyleVar(); 
		ImGui::EndGroup(); 
		ImGui::SameLine(); 
		ImGui::BeginGroup(); 
		ed::PushStyleVar(ed::StyleVar_PivotAlignment, ImVec2(1.0f, 0.5f));
	}

	void ImEdNode::ImEdEndColumn() 
	{ 
		ed::PopStyleVar(); 
		ImGui::EndGroup(); 
	}

}