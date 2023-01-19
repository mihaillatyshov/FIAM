#include "LayerStack.h"

namespace LM
{

	LayerStack::~LayerStack()
	{
		OnDetach();
	}

	void LayerStack::PushLayer(Ref<Layer> _Layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayersInsertIndex, _Layer);
		m_LayersInsertIndex++;
	}

	void LayerStack::PushOverlay(Ref<Layer> _Layer)
	{
		m_Layers.emplace(m_Layers.end(), _Layer);
	}
	
	void LayerStack::PopLayer(Ref<Layer> _Layer)
	{
		auto It = std::find(m_Layers.begin(), m_Layers.begin() + m_LayersInsertIndex, _Layer);
		if (It != m_Layers.begin() + m_LayersInsertIndex)
		{
			_Layer->OnDetach();
			m_Layers.erase(It);
			m_LayersInsertIndex--;
		}
	}

	void LayerStack::PopOverpaly(Ref<Layer> _Layer)
	{
		auto It = std::find(m_Layers.begin() + m_LayersInsertIndex, m_Layers.end(), _Layer);
		if (It != m_Layers.end())
		{
			_Layer->OnDetach();
			m_Layers.erase(It);
		}
	}

	void LayerStack::OnAttach()
	{
		for (auto& Lay : m_Layers)
		{
			Lay->OnAttach();
		}
	}

	void LayerStack::OnDetach()
	{
		for (auto& Lay : m_Layers)
		{
			Lay->OnDetach();
		}
	}

	void LayerStack::OnUpdate()
	{
		for (auto& Lay : m_Layers)
		{
			Lay->OnUpdate();
		}
	}

	void LayerStack::OnEvent(Ref<Event> _Event)
	{
		for (auto& Lay : m_Layers)
		{
			Lay->OnEvent(_Event);
		}
	}




}