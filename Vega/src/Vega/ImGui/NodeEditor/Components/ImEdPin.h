#pragma once

#include "Vega/ImGui/NodeEditor/imgui_node_editor.h"

namespace ed = ax::NodeEditor;

namespace LM
{

	enum class ImEdPinType
	{
		INT,
		FLOAT,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		MAT3,
		MAT4,
	};

	struct ImEdPinProps
	{
		std::string Name;
		ImEdPinType Type;
	};

	class ImEdPin
	{
	public:
		ImEdPin(ImEdPinProps _Props, ed::PinKind _Kind) 
			: m_Props(_Props), m_Kind(_Kind) { }

		int GetID() const { return (int)this; }
		const std::string& GetName() const { return m_Props.Name; }
	protected:
		ax::NodeEditor::PinKind m_Kind;
		ImEdPinProps m_Props;
	};

}