#pragma once

namespace LM
{

	class NodeEditor
	{
	public:
		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
	};

}