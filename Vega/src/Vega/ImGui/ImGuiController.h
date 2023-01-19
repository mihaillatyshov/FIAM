#pragma once

#include "ImGuiImpl.h"
#include "NodeEditor.h"
#include "ImGuiMainWindow.h"
#include "Vega/FIAM/FIAM.h"

namespace LM
{

	class ImGuiController
	{
	public:
		ImGuiController();
		virtual ~ImGuiController();

		virtual void Begin();
		virtual void End();

		virtual void OnUpdate();
	protected:
		Ref<ImGuiImpl> m_ImGuiImpl;

		Ref<NodeEditor> m_NodeEditor;

		Ref<ImGuiMainWindow> m_MainWindow;

		Ref<FIAM> m_FIAM;
	};

}