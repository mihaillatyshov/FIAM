#pragma once

#include <vector>
#include <string>

#include "ImEdPin.h"
#include "ImEdLink.h"

namespace ed = ax::NodeEditor;

namespace LM
{

	class ImEdNode
	{
	public:
		ImEdNode(std::string_view _Name, const std::initializer_list<ImEdPinProps>& _InPins, const std::initializer_list<ImEdPinProps>& _OutPins);
		int GetID() const { return (int)this; }
		void Draw();

		void ImEdBeginColumn();
		void ImEdNextColumn();
		void ImEdEndColumn();
	protected:
		std::string m_Name;
		std::vector<ImEdPin> m_InPins;
		std::vector<ImEdPin> m_OutPins;
		std::vector<ImEdLink> m_Links;

		bool m_FirstFrame = true;
	};

}